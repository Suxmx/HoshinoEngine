#include "Hoshino/Application.h"

#include "Hoshino/Log.h"
#include "Hoshino/Renderer/DeviceManager.h"

#include "Hoshino/TimeStep.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Hoshino
{
	Application* Application::s_Instance;

	Application::Application()
	{
		CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;
		auto deviceMgr = DeviceManager::Create(GraphicsAPI::Vulkan);
		DeviceParameters deviceParams;
		deviceParams.enableDebugRuntime = true;
		deviceMgr->CreateWindowDeviceAndSwapChain(deviceParams);
		// m_Window = Window::Create();
		// m_Window->SetEventCallbackFn(BIND_APP_EVENT_FN(OnEvent));
		// m_Running = true;

		// m_ImGuiLayer = new ImGuiLayer();
		// PushOverlay(m_ImGuiLayer);
	}

	Application::~Application() {}

	void Application::Run()
	{
		while (m_Running)
		{
		//	float time = (float)glfwGetTime();
		//	Timestep timestep = time - m_LastFrameTime;
		//	m_LastFrameTime = time;

		//	// glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//	for (Layer* layer : m_LayerStack)
		//		layer->OnUpdate(timestep);

		//	m_ImGuiLayer->Begin();
		//	for (Layer* layer : m_LayerStack)
		//		layer->OnImGuiRender();
		//	m_ImGuiLayer->End();

		//	m_Window->OnUpdate();
		glfwPollEvents();
		}
		//CORE_INFO("Engine ShutDown!");
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APP_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizedEvent>(BIND_APP_EVENT_FN(OnWindowResize));

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
	
	bool Application::OnWindowResize(WindowResizedEvent& event)
	{
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