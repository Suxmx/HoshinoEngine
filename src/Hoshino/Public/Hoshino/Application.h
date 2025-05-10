#pragma once
#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Graphics/Buffer.h"
#include "Hoshino/Graphics/VertexArray.h"


#include "Hoshino/Window/Window.h"
#include "Hoshino/Layer/LayerStack.h"
#include "Hoshino/Layer/ImGuiLayer.h"
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

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	private:
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
	};
} // namespace Hoshino