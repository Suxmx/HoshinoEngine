#pragma once
#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Layer/ImGuiLayer.h"
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
		inline Window& GetWindow()
		{
			return *m_Window;
		}

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running;
		static Application* s_Instance;

		unsigned int m_VertexArray,m_VertextBuffer,m_IndexBuffer;

	private:
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
	};
} // namespace Hoshino