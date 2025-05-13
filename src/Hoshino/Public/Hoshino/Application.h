#pragma once
#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Graphics/Buffer.h"
#include "Hoshino/Graphics/VertexArray.h"
#include "Hoshino/Graphics/Shader.h"
#include "Hoshino/Graphics/Camera.h"

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
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running;
		static Application* s_Instance;

		Ref<VertexArray> m_TriangleVa;
		Ref<VertexArray> m_SquareVa;
		Ref<Shader> m_Shader;
		Ref<Shader> m_BlueShader;
		Ref<Camera> m_Camera;

	private:
		void OnEvent(Event& event);
		bool OnWindowClose(WindowCloseEvent& event);
	};
} // namespace Hoshino