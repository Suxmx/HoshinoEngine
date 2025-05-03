#pragma once
#include "Core/HoshinoCore.h"
#include "Platform/Window/Window.h"
#include "Event/Event.h"
#include "Event/WindowEvent.h"

namespace Hoshino
{
	class HOSHINO_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running;
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
	};
	Application* CreateApplication();
} // namespace Hoshino