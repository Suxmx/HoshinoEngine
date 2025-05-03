#pragma once
#include "Platform/Window/Window.h"
#include "GLFW/glfw3.h"
namespace Hoshino
{
	class HOSHINO_API WindowsWindow : public Window
	{
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		WindowsWindow(const WindowProps& windowProps) : Window(windowProps)
		{
			Init(windowProps);
		}
		virtual ~WindowsWindow() {}
		unsigned int GetWidth() const override
		{
			return m_Data.Width;
		}
		unsigned int GetHeight() const override
		{
			return m_Data.Height;
		}

		void OnUpdate() override;
		bool IsVSync() override;
		void SetVSync(bool enable) override;
		inline void SetEventCallbackFn(const EventCallbackFn& fn) override
		{
			m_Data.EventCallback = fn;
		}

	private:
		void Init(const WindowProps& props);
		void InitGlfwCallbacks();
		void Shutdown();

		struct WindowData
		{
			unsigned int Height;
			unsigned int Width;
			bool VSync;
			std::string Title;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
		GLFWwindow* m_GlfwWindow;
	};
} // namespace Hoshino