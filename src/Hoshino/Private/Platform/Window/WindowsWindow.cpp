#include "Core/Assert.h"
#include "Core/Log.h"
#include "GLFW/glfw3.h"
#include "Platform/Window/Windows/WindowsWindow.h"

namespace Hoshino
{
	static bool s_GlfwInitialized = false;

	using EventCallbackFn = std::function<void(Event&)>;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	// public
	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_GlfwWindow);
        if(glfwWindowShouldClose(m_GlfwWindow))
        {

        }
	}
	bool WindowsWindow::IsVSync()
	{
		return m_Data.VSync;
	}
	void WindowsWindow::SetVSync(bool enable) {
        if(enable)
        {
            glfwSwapInterval(1);
        }
        else 
        {
            glfwSwapInterval(0);
        }
        m_Data.VSync = enable;
    }

	// private
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Height = props.Height;
		m_Data.Width = props.Width;
		m_Data.Title = props.Title;

		// init glfw
		if (!s_GlfwInitialized)
		{
			int success = glfwInit();
			CORE_ASSERT(success == GLFW_TRUE, "Glfw Init Failed !");
			s_GlfwInitialized = true;
            CORE_INFO("Glfw Init Success !");
		}
		CORE_INFO("Create Window : Width {0}, Height: {1}, Title: {2}",m_Data.Width,m_Data.Height,m_Data.Title);
		m_GlfwWindow = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_GlfwWindow);
		glfwSetWindowUserPointer(m_GlfwWindow, &m_Data);
        SetVSync(true);
        CORE_INFO("Create Window Success !");
	}
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_GlfwWindow);
	}
} // namespace Hoshino