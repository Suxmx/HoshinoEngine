#include "Core/Application.h"
#include "Event/Event.h"
#include "Core/Log.h"
#include "Event/WindowEvent.h"

#define BIND_APP_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Hoshino
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallbackFn(BIND_APP_EVENT_FN(OnEvent));
		m_Running = true;
	}
	Application::~Application() {}
	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
	Application* CreateApplication()
	{
		return new Application();
	}

	void Application::OnEvent(Event& event)
	{
		APP_INFO("App OnEvent : {0}", event.ToString());
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_APP_EVENT_FN(OnWindowClose));
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
} // namespace Hoshino