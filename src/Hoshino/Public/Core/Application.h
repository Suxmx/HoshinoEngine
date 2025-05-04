#pragma once
#include "Core/HoshinoCore.h"
#include "Core/Window/Window.h"
#include "Core/Layer/LayerStack.h"

namespace Hoshino
{
	class HOSHINO_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running;
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
	};
} // namespace Hoshino