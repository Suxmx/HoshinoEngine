#include "Akane/Layer/RenderLayer.h"
#include "Hoshino/Application.h"
using namespace Hoshino;

namespace Akane
{
	void RenderLayer::OnAttach()
	{

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";
		std::string blueFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0, 0,1,1);
			}
		)";

		m_Shader = Shader::Create(vertexSrc, fragmentSrc);
		m_BlueShader = Shader::Create(vertexSrc, blueFragmentSrc);
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

		// VBO
		float squareVertices[3 * 4] = {-0.5f, -0.5f, 0, 0.5, -0.5, 0, 0.5, 0.5, 0, -0.5, 0.5, 0};
		auto squareVb = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVb->Bind();
		squareVb->SetLayout(layout);
		m_SquareVa->AddVertexBuffer(squareVb);

		// EBO
		unsigned int squareIndices[6] = {0, 1, 2, 2, 3, 0};
		auto squareEb = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		squareEb->Bind();
		m_SquareVa->AddIndexBuffer(squareEb);
	}

	void RenderLayer::OnDetach() {}

	void RenderLayer::OnUpdate(Hoshino::Timestep ts)
	{
		RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
		RenderCommand::Clear();
		Renderer::BeginScene(Hoshino::Application::Instance().GetCamera());
		Renderer::Submit(m_SquareVa, m_BlueShader);

		Renderer::Submit(m_TriangleVa, m_Shader);
		Renderer::EndScene();
	}

	void RenderLayer::OnImGuiRender() {}

	void RenderLayer::OnEvent(Hoshino::Event& event) {}
} // namespace Akane