#pragma once
#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Window/Window.h"
#include "Hoshino/Layer/LayerStack.h"

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
		inline static Application& Instance()
		{
			return *s_Instance;
		}
		inline Window& GetWindow();

	private:
		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;
		bool m_Running;
		static Application* s_Instance;
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
	};
} // namespace Hoshino