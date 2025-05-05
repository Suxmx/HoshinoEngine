#include "Core/Application.h"
#include "Core/Event/Event.h"
#include "Core/HoshinoCore.h"
#include "Core/Log.h"
#include "Core/Event/WindowEvent.h"
#include "glad/glad.h"

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Hoshino
{
	Application* Application::s_Instance;

	Application::Application()
	{
		CORE_ASSERT(!s_Instance, "Application already exists!")
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallbackFn(BIND_APP_EVENT_FN(OnEvent));
		m_Running = true;
	}

	Application::~Application() {}

	Window& Application::GetWindow()
	{
		return *m_Window;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			m_Window->OnUpdate();
		}
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