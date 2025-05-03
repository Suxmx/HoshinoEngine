#include "Core/Assert.h"
#include "Core/Log.h"
#include "Event/MouseEvent.h"
#include "GLFW/glfw3.h"
#include "Platform/Window/Windows/WindowsWindow.h"
// Events
#include "Event/WindowEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

namespace Hoshino
{
	static bool s_GlfwInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

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
		if (glfwWindowShouldClose(m_GlfwWindow))
		{
		}
	}
	bool WindowsWindow::IsVSync()
	{
		return m_Data.VSync;
	}
	void WindowsWindow::SetVSync(bool enable)
	{
		if (enable)
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
			glfwSetErrorCallback(GLFWErrorCallback);
			CORE_INFO("Glfw Init Success !");
		}
		CORE_INFO("Create Window : Width {0}, Height: {1}, Title: {2}", m_Data.Width, m_Data.Height,
		          m_Data.Title);
		m_GlfwWindow = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_GlfwWindow);
		glfwSetWindowUserPointer(m_GlfwWindow, &m_Data);
		SetVSync(true);
		CORE_INFO("Create Window Success !");
		InitGlfwCallbacks();
	}

	void WindowsWindow::InitGlfwCallbacks()
	{
		glfwSetWindowSizeCallback(m_GlfwWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizedEvent event(width, height);
			data.EventCallback(event);
		});
		glfwSetWindowCloseCallback(m_GlfwWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			if (data.EventCallback) data.EventCallback(event);
		});
		glfwSetKeyCallback(m_GlfwWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyCode keyCode = static_cast<KeyCode>(key);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent pressEvent(keyCode, 0);
				if (data.EventCallback) data.EventCallback(pressEvent);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent pressEvent(keyCode, 1);
				if (data.EventCallback) data.EventCallback(pressEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent pressEvent(keyCode);
				if (data.EventCallback) data.EventCallback(pressEvent);
				break;
			}
			}
		});
		glfwSetMouseButtonCallback(m_GlfwWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				if (data.EventCallback) data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				if (data.EventCallback) data.EventCallback(event);
				break;
			}
			}
		});
		glfwSetScrollCallback(m_GlfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_GlfwWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});
	}
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_GlfwWindow);
	}
} // namespace Hoshino