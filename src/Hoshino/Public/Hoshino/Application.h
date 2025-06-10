#pragma once
#include "Hoshino/HoshinoCore.h"

#include "Hoshino/Window/Window.h"
#include "Hoshino/Layer/LayerStack.h"
#include "Hoshino/Layer/ImGuiLayer.h"
#include "Hoshino/Renderer/DeviceManager.h"
#include <nvrhi/nvrhi.h>
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
		inline DeviceManager* GetDeviceManager()
		{
			return m_Window->GetDeviceManager();
		}
		void OnEvent(Event& event);

	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running;
		float m_LastFrameTime = 0.0f;
		static Application* s_Instance;
		nvrhi::CommandListHandle m_CommandList;

	protected:

	private:
		
		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizedEvent& event);
	};
} // namespace Hoshino