// From Hazel
#include "Hoshino/Asset/AssetImporter.h"
#include "Hoshino/Graphics/BufferLayout.h"
#include "Hoshino/Graphics/Material.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <glm/glm.hpp>

namespace Hoshino
{
	static const uint32_t s_MeshImportFlags =
	    aiProcess_CalcTangentSpace // Create binormals/tangents just in case
	    | aiProcess_Triangulate    // Make sure we're triangles
	    | aiProcess_SortByPType    // Split meshes by primitive type
	    | aiProcess_GenNormals     // Make sure we have legit normals
	    | aiProcess_GenUVCoords    // Convert UVs if required
	                               //		| aiProcess_OptimizeGraph
	    | aiProcess_OptimizeMeshes // Batch draws where possible
	    | aiProcess_JoinIdenticalVertices |
	    aiProcess_LimitBoneWeights // If more than N (=4) bone weights, discard least influencing bones and
	                               // renormalise sum to 1
	    | aiProcess_ValidateDataStructure // Validation
	    | aiProcess_GlobalScale // e.g. convert cm to m for fbx import (and other formats where cm is native)
	    ;

	static glm::mat4 Mat4FromAIMatrix4x4(const aiMatrix4x4& matrix)
	{
		glm::mat4 result;
		// the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		result[0][0] = matrix.a1;
		result[1][0] = matrix.a2;
		result[2][0] = matrix.a3;
		result[3][0] = matrix.a4;
		result[0][1] = matrix.b1;
		result[1][1] = matrix.b2;
		result[2][1] = matrix.b3;
		result[3][1] = matrix.b4;
		result[0][2] = matrix.c1;
		result[1][2] = matrix.c2;
		result[2][2] = matrix.c3;
		result[3][2] = matrix.c4;
		result[0][3] = matrix.d1;
		result[1][3] = matrix.d2;
		result[2][3] = matrix.d3;
		result[3][3] = matrix.d4;
		return result;
	}
	Ref<MeshSource> AssetImporter::ImportMesh(const std::string& path)
	{
		auto meshSource = CreateRef<MeshSource>();
		CORE_INFO("Loading mesh: {0}", path);

		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		const aiScene* scene = importer.ReadFile(path, s_MeshImportFlags);
		if (!scene /* || !scene->HasMeshes()*/) // note: scene can legit contain no meshes (e.g. it could
		                                        // contain an armature, an animation, and no skin (mesh)))
		{
			CORE_ERROR("Mesh", "Failed to load mesh file: {0}", path);
			return nullptr;
		}

#pragma region Mesh
		// If no meshes in the scene, there's nothing more for us to do
		if (scene->HasMeshes())
		{
			uint32_t vertexCount = 0;
			uint32_t indexCount = 0;

			meshSource->m_Submeshes.reserve(scene->mNumMeshes);
			for (unsigned m = 0; m < scene->mNumMeshes; m++)
			{
				aiMesh* mesh = scene->mMeshes[m];

				if (!mesh->HasPositions())
				{
					CORE_WARN("Mesh index {0} with name '{1}' has no vertex positions - skipping import!", m,
					          mesh->mName.C_Str());
				}
				if (!mesh->HasNormals())
				{
					CORE_WARN("Mesh index {0} with name '{1}' has no vertex normals, and they "
					          "could not be computed - skipping import!",
					          m, mesh->mName.C_Str());
				}

				bool skip = !mesh->HasPositions() || !mesh->HasNormals();

				// still have to create a placeholder submesh even if we are skipping it (otherwise
				// TraverseNodes() does not work)
				Submesh& submesh = meshSource->m_Submeshes.emplace_back();
				submesh.BaseVertex = vertexCount;
				submesh.BaseIndex = indexCount;
				submesh.MaterialIndex = mesh->mMaterialIndex;
				submesh.VertexCount = skip ? 0 : mesh->mNumVertices;
				submesh.IndexCount = skip ? 0 : mesh->mNumFaces * 3;
				submesh.MeshName = mesh->mName.C_Str();

				if (skip) continue;

				vertexCount += mesh->mNumVertices;
				indexCount += submesh.IndexCount;

				// Vertices
				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					Vertex vertex;
					vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
					vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

					if (mesh->HasTangentsAndBitangents())
					{
						vertex.Tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
						vertex.Binormal = {mesh->mBitangents[i].x, mesh->mBitangents[i].y,
						                   mesh->mBitangents[i].z};
					}

					if (mesh->HasTextureCoords(0))
						vertex.Texcoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};

					meshSource->m_Vertices.push_back(vertex);
				}

				// Indices
				for (size_t i = 0; i < mesh->mNumFaces; i++)
				{
					// we're using aiProcess_Triangulate so this should always be true
					CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
					Index index = {mesh->mFaces[i].mIndices[0] + submesh.BaseVertex,
					               mesh->mFaces[i].mIndices[1] + submesh.BaseVertex,
					               mesh->mFaces[i].mIndices[2] + submesh.BaseVertex};
					meshSource->m_Indices.push_back(index);

					meshSource->m_TriangleCache[m].emplace_back(
					    meshSource->m_Vertices[index.V1],
					    meshSource->m_Vertices[index.V2],
					    meshSource->m_Vertices[index.V3]);
				}
			}

#if MESH_DEBUG_LOG
			CORE_INFO("Mesh", "Traversing nodes for scene '{0}'", m_Path);
			// Utils::PrintNode(scene->mRootNode, 0);
#endif

			MeshNode& rootNode = meshSource->m_Nodes.emplace_back();
			TraverseNodes(meshSource, scene->mRootNode, 0);
		}
#pragma endregion Mesh

#pragma region Material
		if (scene->HasMaterials())
		{
			CORE_INFO("---- Materials - {0} ----", path);

			meshSource->m_Materials.resize(scene->mNumMaterials);

			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				auto aiMaterial = scene->mMaterials[i];
				auto aiMaterialName = aiMaterial->GetName();
				Ref<Material> material = Material::Create();

				CORE_INFO("  {0} (Index = {1})", aiMaterialName.data, i);

				glm::vec3 albedoColor(0.8f);
				float emission = 0.0f;
				aiColor3D aiColor, aiEmission;
				if (aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor) == AI_SUCCESS)
					albedoColor = {aiColor.r, aiColor.g, aiColor.b};

				if (aiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmission) == AI_SUCCESS)
					emission = aiEmission.r;

				material->Set("u_AlbedoColor", albedoColor);
				material->Set("u_Emission", emission);

				float roughness, metalness;
				if (aiMaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughness) != aiReturn_SUCCESS)
					roughness = 0.4f; // Default value

				if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS) metalness = 0.0f;

				// Physically realistic materials are either metal (1.0) or not (0.0)
				// Some models seem to come in with 0.5 which seems wrong - materials are either metal or they
				// are not. (maybe these are specular workflow, and what we're seeing is specular = 0.5 in
				// AI_MATKEY_REFLECTIVITY (?))
				if (metalness < 0.9f) metalness = 0.0f;
				else metalness = 1.0f;

				material->Set("u_Roughness", roughness);
				material->Set("u_Metalness", metalness);

				CORE_TRACE("    COLOR = {0}, {1}, {2}", aiColor.r, aiColor.g, aiColor.b);
				CORE_TRACE("    EMISSION = {0}", emission);
				CORE_TRACE("    ROUGHNESS = {0}", roughness);
				CORE_TRACE("    METALNESS = {0}", metalness);

				bool metalnessTextureFound = false;
				// CORE_INFO("Material Property:");
				for (uint32_t p = 0; p < aiMaterial->mNumProperties; p++)
				{
					auto prop = aiMaterial->mProperties[p];

					float data = *(float*)prop->mData;
					// CORE_INFO("   Name = {0}, Value = {1}", prop->mKey.data, data);
				}
				meshSource->m_Materials[i] = material;
			}
			CORE_INFO("------------------------"); 
		}
#pragma endregion Material
		meshSource->m_VertexArray = VertexArray::Create();
		meshSource->m_VertexArray->Bind();

		if (meshSource->m_Vertices.size())
		{
			meshSource->m_VertexBuffer = VertexBuffer::Create(
			    meshSource->m_Vertices.data(), (uint32_t)(meshSource->m_Vertices.size() * sizeof(Vertex)));
			meshSource->m_VertexBuffer->Bind();
			BufferLayout layout = {
			    {"a_Position", ShaderDataType::Float3}, {"a_Normal", ShaderDataType::Float3},
			    {"a_Tangent", ShaderDataType::Float3},  {"a_Binormal", ShaderDataType::Float3},
			    {"a_Texcoord", ShaderDataType::Float2},
			};
			meshSource->m_VertexBuffer->SetLayout(layout);
			meshSource->m_VertexBuffer->Bind();
			meshSource->m_VertexArray->AddVertexBuffer(meshSource->m_VertexBuffer);
		}

		if (meshSource->m_Indices.size())
		{
			meshSource->m_IndexBuffer = IndexBuffer::Create(
			    meshSource->m_Indices.data(), (uint32_t)(meshSource->m_Indices.size() * sizeof(Index)));
			meshSource->m_IndexBuffer->Bind();
			meshSource->m_VertexArray->AddIndexBuffer(meshSource->m_IndexBuffer);
		}
		meshSource->m_IndexBuffer->Bind();
		meshSource->m_VertexArray->Unbind();

		return meshSource;
	}

	void AssetImporter::TraverseNodes(Ref<MeshSource> meshSource, void* assimpNode, uint32_t nodeIndex,
	                                  const glm::mat4& parentTransform, uint32_t level)
	{
		aiNode* aNode = (aiNode*)assimpNode;

		MeshNode& node = meshSource->m_Nodes[nodeIndex];
		node.Name = aNode->mName.C_Str();
		node.LocalTransform = Mat4FromAIMatrix4x4(aNode->mTransformation);

		glm::mat4 transform = parentTransform * node.LocalTransform;
		for (uint32_t i = 0; i < aNode->mNumMeshes; i++)
		{
			uint32_t submeshIndex = aNode->mMeshes[i];
			auto& submesh = meshSource->m_Submeshes[submeshIndex];
			submesh.NodeName = aNode->mName.C_Str();
			submesh.Transform = transform;
			submesh.LocalTransform = node.LocalTransform;

			node.Submeshes.push_back(submeshIndex);
		}

		// CORE_INFO("{0} {1}", LevelToSpaces(level), node->mName.C_Str());

		uint32_t parentNodeIndex = (uint32_t)meshSource->m_Nodes.size() - 1;
		node.Children.resize(aNode->mNumChildren);
		for (uint32_t i = 0; i < aNode->mNumChildren; i++)
		{
			MeshNode& child = meshSource->m_Nodes.emplace_back();
			uint32_t childIndex = static_cast<uint32_t>(meshSource->m_Nodes.size()) - 1;
			child.Parent = parentNodeIndex;
			meshSource->m_Nodes[nodeIndex].Children[i] = childIndex;
			TraverseNodes(meshSource, aNode->mChildren[i], childIndex, transform, level + 1);
		}
	}
} // namespace Hoshino