#include "Hoshino/Application.h"
#include "Hoshino/Graphics/Buffer.h"
#include "Hoshino/Log.h"
#include "Hoshino/Graphics/Renderer.h"
#include "Hoshino/Graphics/RenderCommand.h"

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Hoshino
{
	Application* Application::s_Instance;

	Application::Application()
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

		CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;
		m_Window = Window::Create();
		m_Window->SetEventCallbackFn(BIND_APP_EVENT_FN(OnEvent));
		m_Running = true;

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		m_Shader = Shader::Create(vertexSrc, fragmentSrc);
		m_BlueShader = Shader::Create(vertexSrc, blueFragmentSrc);
		//VAO
		m_SquareVa = VertexArray::Create();
		m_TriangleVa = VertexArray::Create();
		//VBO
		float triangleVertices[3 * 3] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
		auto triangleVb = VertexBuffer::Create(triangleVertices, sizeof(triangleVertices));
		triangleVb->Bind();
		BufferLayout layout = {{"a_Position", ShaderDataType::Float3}};
		triangleVb->SetLayout(layout);
		m_TriangleVa->AddVertexBuffer(triangleVb);

		// EBO
		unsigned int triangleIndices[3] = {0, 1, 2};
		auto triangleEb=IndexBuffer::Create(triangleIndices,sizeof(triangleIndices)/sizeof(uint32_t));
		triangleEb->Bind();
		m_TriangleVa->AddIndexBuffer(triangleEb);

		// VBO
		float squareVertices[3 * 4] = {-0.5f, -0.5f, 0, 0.5, -0.5, 0, 0.5, 0.5, 0, -0.5, 0.5, 0};
		auto squareVb = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVb->Bind();
		squareVb->SetLayout(layout);
		m_SquareVa->AddVertexBuffer(squareVb);

		// EBO
		unsigned int squareIndices[6] = {0, 1, 2,2,3,0};
		auto squareEb = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		squareEb->Bind();
		m_SquareVa->AddIndexBuffer(squareEb);
		m_Camera = std::make_shared<OrthographicCamera>(1.5f, -1.0f, 1.0f);
		m_Camera->SetRotationEuler(0,0,45);
		m_Camera->SetPosition(0.2f, 0.1f, 0);
	}

	Application::~Application() {}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1));
			RenderCommand::Clear();
			Renderer::BeginScene(m_Camera);
			Renderer::Submit(m_SquareVa,m_BlueShader);

			Renderer::Submit(m_TriangleVa,m_Shader);
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
		CORE_INFO("Engine ShutDown!");
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APP_EVENT_FN(OnWindowClose));

		if (event.Handled) return;

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(event);
			if (event.Handled) break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

#pragma region LayerStack
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PopLayer(Layer* layer)
	{
		m_LayerStack.PopLayer(layer);
		layer->OnDetach();
	}

	void Application::PopOverlay(Layer* overlay)
	{
		m_LayerStack.PopOverlay(overlay);
		overlay->OnDetach();
	}
#pragma endregion
} // namespace Hoshino