#include "Hoshino/Graphics/Mesh.h"
#include "Hoshino/Graphics/BufferLayout.h"
#include "Hoshino/Graphics/Material.h"

namespace Hoshino
{
	Ref<MeshSource> MeshSource::CreateScreenQuad()
	{
		// 创建顶点数据 - 注意顶点位置使用NDC坐标系（-1到1）
		std::vector<Vertex> vertices = {// 左下
		                                {
		                                    glm::vec3(-1.0f, -1.0f, 0.0f), // 位置
		                                    glm::vec3(0.0f, 0.0f, 1.0f),   // 法线
		                                    glm::vec3(1.0f, 0.0f, 0.0f),   // 切线
		                                    glm::vec3(0.0f, 1.0f, 0.0f),   // 副切线
		                                    glm::vec2(0.0f, 0.0f)          // 纹理坐标 (0,0)
		                                },
		                                // 右下
		                                {
		                                    glm::vec3(1.0f, -1.0f, 0.0f), // 位置
		                                    glm::vec3(0.0f, 0.0f, 1.0f),  // 法线
		                                    glm::vec3(1.0f, 0.0f, 0.0f),  // 切线
		                                    glm::vec3(0.0f, 1.0f, 0.0f),  // 副切线
		                                    glm::vec2(1.0f, 0.0f)         // 纹理坐标 (1,0)
		                                },
		                                // 右上
		                                {
		                                    glm::vec3(1.0f, 1.0f, 0.0f), // 位置
		                                    glm::vec3(0.0f, 0.0f, 1.0f), // 法线
		                                    glm::vec3(1.0f, 0.0f, 0.0f), // 切线
		                                    glm::vec3(0.0f, 1.0f, 0.0f), // 副切线
		                                    glm::vec2(1.0f, 1.0f)        // 纹理坐标 (1,1)
		                                },
		                                // 左上
		                                {
		                                    glm::vec3(-1.0f, 1.0f, 0.0f), // 位置
		                                    glm::vec3(0.0f, 0.0f, 1.0f),  // 法线
		                                    glm::vec3(1.0f, 0.0f, 0.0f),  // 切线
		                                    glm::vec3(0.0f, 1.0f, 0.0f),  // 副切线
		                                    glm::vec2(0.0f, 1.0f)         // 纹理坐标 (0,1)
		                                }};

		// 创建索引数据 - 两个三角形构成一个四边形
		std::vector<Index> indices = {
		    {.V1 = 0, .V2 = 1, .V3 = 2}, // 第一个三角形
		    {.V1 = 2, .V2 = 3, .V3 = 0}  // 第二个三角形
		};

		// 创建子网格
		Submesh submesh;
		submesh.BaseVertex = 0;
		submesh.BaseIndex = 0;
		submesh.IndexCount = 6; // 2个三角形，每个3个索引
		submesh.VertexCount = 4;
		submesh.MaterialIndex = 0;
		submesh.Transform = glm::mat4(1.0f); // 单位矩阵
		submesh.MeshName = "ScreenQuad";
		submesh.NodeName = "ScreenQuad";

		std::vector<Submesh> submeshes = {submesh};

		// 创建MeshSource
		Ref<MeshSource> meshSource = CreateRef<MeshSource>(vertices, indices, submeshes);

		// 创建节点
		MeshNode node;
		node.Name = "ScreenQuad";
		node.LocalTransform = glm::mat4(1.0f);
		node.Submeshes = {0};
		meshSource->m_Nodes.push_back(node);

		// 设置顶点缓冲区和索引缓冲区
		BufferLayout layout = {{"a_Position", ShaderDataType::Float3},
		                       {"a_Normal", ShaderDataType::Float3},
		                       {"a_Tangent", ShaderDataType::Float3},
		                       {"a_Binormal", ShaderDataType::Float3},
		                       {"a_TexCoord", ShaderDataType::Float2}};

		meshSource->m_VertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
		meshSource->m_VertexBuffer->SetLayout(layout);

		meshSource->m_IndexBuffer =
		    IndexBuffer::Create(indices.data(), (uint32_t)(indices.size() * sizeof(Index)));

		meshSource->m_VertexArray = VertexArray::Create();
		meshSource->m_VertexBuffer->Bind();
		meshSource->m_VertexArray->AddVertexBuffer(meshSource->m_VertexBuffer);
		meshSource->m_IndexBuffer->Bind();
		meshSource->m_VertexArray->AddIndexBuffer(meshSource->m_IndexBuffer);

		return meshSource;
	}
} // namespace Hoshino