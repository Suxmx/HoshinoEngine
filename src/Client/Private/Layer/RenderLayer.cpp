#include "Akane/Layer/RenderLayer.h"
#include "Akane/SandboxApplication.h"
#include "Hoshino/Application.h"
using namespace Hoshino;

namespace Akane
{
	void RenderLayer::OnAttach()
	{
		m_Shader = Shader::CreateFromFile("Vert/Vertex.vert", "Frag/Normal.glsl");
		m_BlueShader = Shader::CreateFromFile("Vert/Vertex.vert", "Frag/Blue.glsl");
		// VAO
		m_SquareVa = VertexArray::Create();
		m_TriangleVa = VertexArray::Create();
		// VBO
		float triangleVertices[3 * 3] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
		auto triangleVb = VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));
		triangleVb->Bind();
		BufferLayout layout = {{"a_Position", ShaderDataType::Float3}};
		triangleVb->SetLayout(layout);
		m_TriangleVa->AddVertexBuffer(triangleVb);

		// EBO
		unsigned int triangleIndices[3] = {0, 1, 2};
		auto triangleEb = IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(uint32_t));
		triangleEb->Bind();
		m_TriangleVa->AddIndexBuffer(triangleEb);

		BufferLayout sqrLayout = {{"a_Position", ShaderDataType::Float3},
		                          {"a_Texcoord", ShaderDataType::Float2}};
		// VBO
		float squareVertices[5 * 4] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
		                               0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f};
		auto squareVb = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVb->Bind();
		squareVb->SetLayout(sqrLayout);
		m_SquareVa->AddVertexBuffer(squareVb);

		// EBO
		unsigned int squareIndices[6] = {0, 1, 2, 2, 3, 0};
		auto squareEb = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		squareEb->Bind();
		m_SquareVa->AddIndexBuffer(squareEb);

		//Texture
		m_Texture = Texture::Create("Res/Texture/test.png");
	}

	void RenderLayer::OnDetach() {}

	void RenderLayer::OnUpdate(Hoshino::Timestep ts)
	{
		SandboxApplication& app = static_cast<SandboxApplication&>(Hoshino::Application::Instance());
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		RenderCommand::Clear();
		Renderer::BeginScene(Hoshino::Application::Instance().GetCamera());
		m_BlueShader->Bind();
		m_Texture->Bind(0);
		m_BlueShader->UploadUniformInt("ourTexture", 0);
		Renderer::Submit(m_SquareVa, m_BlueShader,app.SqrTransform);

		Renderer::Submit(m_TriangleVa, m_Shader,app.TriTransform);
		Renderer::EndScene();
	}

	void RenderLayer::OnImGuiRender() {}

	void RenderLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane