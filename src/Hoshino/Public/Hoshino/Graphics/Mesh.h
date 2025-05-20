#pragma once
#include "HoshinoMin.h"

#include "Hoshino/Graphics/Buffer.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"
#include "VertexArray.h"
namespace Hoshino
{
	class Renderer;
	class Material;
#pragma region MeshNode
	struct MeshNode
	{
		uint32_t Parent = 0xffffffff;
		std::vector<uint32_t> Children;
		std::vector<uint32_t> Submeshes;

		std::string Name;
		glm::mat4 LocalTransform;

		inline bool IsRoot() const
		{
			return Parent == 0xffffffff;
		}
	};
	struct Index
	{
		uint32_t V1, V2, V3;
	};

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Binormal;
		glm::vec2 Texcoord;
	};

	static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

	struct Triangle
	{
		Vertex V0, V1, V2;

		Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2) : V0(v0), V1(v1), V2(v2) {}
	};

	class Submesh
	{
	public:
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;
		uint32_t VertexCount;

		glm::mat4 Transform{1.0f}; // World transform
		glm::mat4 LocalTransform{1.0f};

		std::string NodeName, MeshName;
		bool IsRigged = false;
	};

#pragma endregion MeshNode

#pragma region MeshSource

	class MeshSource
	{
	public:
		MeshSource() = default;
		MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
		           const glm::mat4& transform);
		MeshSource(const std::vector<Vertex>& vertices, const std::vector<Index>& indices,
		           const std::vector<Submesh>& submeshes);
		virtual ~MeshSource(){}

		void DumpVertexBuffer();

		std::vector<Submesh>& GetSubmeshes()
		{
			return m_Submeshes;
		}
		const std::vector<Submesh>& GetSubmeshes() const
		{
			return m_Submeshes;
		}

		const std::vector<Vertex>& GetVertices() const
		{
			return m_Vertices;
		}
		const std::vector<Index>& GetIndices() const
		{
			return m_Indices;
		}
		const std::string& GetFilePath() const
		{
			return m_FilePath;
		}

		const std::vector<Triangle> GetTriangleCache(uint32_t index) const
		{
			return m_TriangleCache.at(index);
		}

		
		Ref<VertexArray> GetVertexArray()
		{
			return m_VertexArray;
		}
		Ref<VertexBuffer> GetVertexBuffer()
		{
			return m_VertexBuffer;
		}
		Ref<IndexBuffer> GetIndexBuffer()
		{
			return m_IndexBuffer;
		}

		const MeshNode& GetRootNode() const
		{
			return m_Nodes[0];
		}
		const std::vector<MeshNode>& GetNodes() const
		{
			return m_Nodes;
		}

	private:
		std::vector<Submesh> m_Submeshes;
		std::vector<Ref<Material>> m_Materials;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<VertexArray> m_VertexArray;

		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

		std::vector<MeshNode> m_Nodes;
		std::string m_FilePath;

        friend class AssetImporter;
		friend class OpenGLRenderAPI;
		friend class Renderer;
		friend class MeshRenderObject;
	};

#pragma endregion MeshSource
} // namespace Hoshino