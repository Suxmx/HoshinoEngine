#include "Hoshino/Window/Window.h"
#include "Hoshino/HoshinoCore.h"
// Platform
#include "Platform/Windows/WindowsKeyCodeWrapper.h"

namespace Hoshino
{
#pragma region Const

	Scope<Window> Window::Create(const WindowSpecification& props)
	{
		return std::make_unique<Window>(props);
	}
	static const struct
	{
		TextureFormat format;
		uint32_t redBits;
		uint32_t greenBits;
		uint32_t blueBits;
		uint32_t alphaBits;
		uint32_t depthBits;
		uint32_t stencilBits;
	} formatInfo[] = {
	    {
	        TextureFormat::R8_UINT,
	        8,
	        0,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RG8_UINT,
	        8,
	        8,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RG8_UNORM,
	        8,
	        8,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::R16_UINT,
	        16,
	        0,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::R16_UNORM,
	        16,
	        0,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::R16_FLOAT,
	        16,
	        0,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGBA8_UNORM,
	        8,
	        8,
	        8,
	        8,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGBA8_SNORM,
	        8,
	        8,
	        8,
	        8,
	        0,
	        0,
	    },
	    {
	        TextureFormat::BGRA8_UNORM,
	        8,
	        8,
	        8,
	        8,
	        0,
	        0,
	    },
	    {
	        TextureFormat::SRGBA8_UNORM,
	        8,
	        8,
	        8,
	        8,
	        0,
	        0,
	    },
	    {
	        TextureFormat::SBGRA8_UNORM,
	        8,
	        8,
	        8,
	        8,
	        0,
	        0,
	    },
	    {
	        TextureFormat::R10G10B10A2_UNORM,
	        10,
	        10,
	        10,
	        2,
	        0,
	        0,
	    },
	    {
	        TextureFormat::R11G11B10_FLOAT,
	        11,
	        11,
	        10,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RG16_UINT,
	        16,
	        16,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RG16_FLOAT,
	        16,
	        16,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::R32_UINT,
	        32,
	        0,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::R32_FLOAT,
	        32,
	        0,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGBA16_FLOAT,
	        16,
	        16,
	        16,
	        16,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGBA16_UNORM,
	        16,
	        16,
	        16,
	        16,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGBA16_SNORM,
	        16,
	        16,
	        16,
	        16,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RG32_UINT,
	        32,
	        32,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RG32_FLOAT,
	        32,
	        32,
	        0,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGB32_UINT,
	        32,
	        32,
	        32,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGB32_FLOAT,
	        32,
	        32,
	        32,
	        0,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGBA32_UINT,
	        32,
	        32,
	        32,
	        32,
	        0,
	        0,
	    },
	    {
	        TextureFormat::RGBA32_FLOAT,
	        32,
	        32,
	        32,
	        32,
	        0,
	        0,
	    },
	};

#pragma endregion Const

	static void GlfwErrorCallback(int error, const char* description)
	{
		CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	void Window::Init()
	{
		DeviceParameters deviceParams;
		deviceParams.swapChainBufferCount = 3;
		deviceParams.maxFramesInFlight = 1;
		deviceParams.backBufferWidth = m_Specification.Width;
		deviceParams.backBufferHeight = m_Specification.Height;
		deviceParams.vsyncEnabled = false;
		deviceParams.enableDebugRuntime = true;

		if (!glfwInit())
		{
			CORE_ERROR("Failed to initialize GLFW");
			return;
		}

		glfwSetErrorCallback(GlfwErrorCallback);
		glfwDefaultWindowHints();

		// 初始化交换链Format的hint属性
		bool foundFormat = false;
		for (const auto& info : formatInfo)
		{
			if (info.format == deviceParams.swapChainFormat)
			{
				glfwWindowHint(GLFW_RED_BITS, info.redBits);
				glfwWindowHint(GLFW_GREEN_BITS, info.greenBits);
				glfwWindowHint(GLFW_BLUE_BITS, info.blueBits);
				glfwWindowHint(GLFW_ALPHA_BITS, info.alphaBits);
				glfwWindowHint(GLFW_DEPTH_BITS, info.depthBits);
				glfwWindowHint(GLFW_STENCIL_BITS, info.stencilBits);
				foundFormat = true;
				break;
			}
		}
		CORE_ASSERT(foundFormat, "Unsupported swap chain format : {0}",
		            static_cast<uint32_t>(deviceParams.swapChainFormat));

		glfwWindowHint(GLFW_REFRESH_RATE, deviceParams.refreshRate);
		glfwWindowHint(GLFW_SAMPLES, deviceParams.swapChainSampleCount);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // 不使用OpenGL
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);     // 创建窗口时不显示
		                                              // 是否无边框
		if (deviceParams.startBorderless)
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		}

		m_GlfwWindow = glfwCreateWindow(deviceParams.backBufferWidth, deviceParams.backBufferHeight,
		                                m_Specification.Title.c_str(),
		                                nullptr, // monitor
		                                nullptr  // share
		);

		if (m_GlfwWindow == nullptr)
		{
			CORE_ERROR("Failed to create GLFW window");
			return;
		}
		// 这一段有点没看懂 可能是为了照顾系统缩放 但是为什么没考虑全屏的？
		if (deviceParams.startFullscreen)
		{
			glfwSetWindowMonitor(m_GlfwWindow, glfwGetPrimaryMonitor(), 0, 0, m_Specification.Width,
			                     m_Specification.Height, deviceParams.refreshRate);
		}
		else
		{
			int fbWidth = 0, fbHeight = 0;
			glfwGetFramebufferSize(m_GlfwWindow, &fbWidth, &fbHeight);
			m_Data.Width = fbWidth;
			m_Data.Height = fbHeight;
		}
		glfwSetWindowUserPointer(m_GlfwWindow, &m_Data);
		InitGlfwCallbacks();
		// 初始化devicemanager
		m_DeviceMgr =
		    std::unique_ptr<DeviceManager>(DeviceManager::Create(GraphicsAPI::Vulkan, m_GlfwWindow));
		if (!m_DeviceMgr->CreateDevice(deviceParams))
		{
			CORE_ERROR("Failed to create DeviceManager");
			return;
		}
		if (deviceParams.startMaximizedWindow)
		{
			glfwMaximizeWindow(m_GlfwWindow);
		}
		glfwShowWindow(m_GlfwWindow);
		// resize swapchain
		// reset the back buffer size state to enforce a resize event
		m_Data.Width = 0;
		m_Data.Height = 0;

		UpdateWindowSize();
	}
	void Window::InitGlfwCallbacks()
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
			KeyCode keyCode = GlfwKeyToKeyCode(key);
			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent pressEvent(keyCode, mods, 0);
				if (data.EventCallback) data.EventCallback(pressEvent);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent pressEvent(keyCode, mods, 1);
				if (data.EventCallback) data.EventCallback(pressEvent);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent pressEvent(keyCode, mods);
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
		glfwSetCharCallback(m_GlfwWindow, [](GLFWwindow* window, unsigned int key)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(key);
			data.EventCallback(event);
		});
	}
	void Window::BeginFrame() {}
	void Window::Present() {}
	void Window::UpdateWindowSize()
	{
		int width, height;
		glfwGetWindowSize(m_GlfwWindow, &width, &height);
		if (width == 0 || height == 0)
		{
			m_Data.WindowVisible = false;
			return;
		}
		m_Data.WindowVisible = false;
		m_Data.WindowFocused = glfwGetWindowAttrib(m_GlfwWindow, GLFW_FOCUSED) == 1;
		if (static_cast<int>(m_Data.Width) != width || static_cast<int>(m_Data.Width) != height ||
		    m_Data.VsyncEnabled != m_Specification.VSync)
		{
			m_Data.Width = width;
			m_Data.Height = height;
			if (m_DeviceMgr)
			{
				m_DeviceMgr->BackBufferResizing();
				m_DeviceMgr->OnResize(m_Data.Width, m_Data.Height);
				m_DeviceMgr->BackBufferResized();
			}
		}
		else
		{
			m_Data.WindowVisible = true;
		}
	}

} // namespace Hoshino