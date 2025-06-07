#include "Hoshino/Renderer/DeviceManager.h"
#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Application.h"
#include "Platform/Vulkan/VulkanDeviceManager.h"

namespace Hoshino
{
#pragma region Const

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

#pragma region GlfwCallback
	static void GlfwErrorCallback(int error, const char* description)
	{
		CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	static void WindowIconifyCallback_GLFW(GLFWwindow* window, int iconified)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		WindowMinimizeEvent event(iconified == GLFW_TRUE);
		app->OnEvent(event);
	}

	static void WindowFocusCallback_GLFW(GLFWwindow* window, int focused)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		if (focused == GLFW_FALSE)
		{
			WindowLostFocusEvent event;
			app->OnEvent(event);
		}
		else
		{
			WindowFocusEvent event;
			app->OnEvent(event);
		}
	}

	static void WindowRefreshCallback_GLFW(GLFWwindow* window)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		WindowRefreshEvent event;
		app->OnEvent(event);
	}

	static void WindowCloseCallback_GLFW(GLFWwindow* window)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		WindowCloseEvent event;
		app->OnEvent(event);
	}

	static void WindowPosCallback_GLFW(GLFWwindow* window, int xpos, int ypos)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		WindowMoveEvent event(xpos, ypos);
		app->OnEvent(event);
	}

	static void WindowSizeCallback_GLFW(GLFWwindow* window, int width, int height)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		WindowResizedEvent event(width, height);
		app->OnEvent(event);
	}

	static void KeyCallback_GLFW(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		KeyCode keyCode = static_cast<KeyCode>(key);

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event(keyCode, mods, 0);
			app->OnEvent(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event(keyCode, mods, 1);
			app->OnEvent(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event(keyCode, mods);
			app->OnEvent(event);
			break;
		}
		}
	}

	static void CharCallback_GLFW(GLFWwindow* window, unsigned int keycode)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		KeyTypedEvent event(keycode);
		app->OnEvent(event);
	}

	static void MousePosCallback_GLFW(GLFWwindow* window, double xpos, double ypos)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		MouseMovedEvent event((float)xpos, (float)ypos);
		app->OnEvent(event);
	}

	static void MouseButtonCallback_GLFW(GLFWwindow* window, int button, int action, int mods)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event(button);
			app->OnEvent(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event(button);
			app->OnEvent(event);
			break;
		}
		}
	}

	static void MouseScrollCallback_GLFW(GLFWwindow* window, double xoffset, double yoffset)
	{
		Application* app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
		MouseScrolledEvent event((float)xoffset, (float)yoffset);
		app->OnEvent(event);
	}
#pragma endregion GlfwCallback

	DeviceManager* DeviceManager::Create(GraphicsAPI api)
	{
		switch (api)
		{
		case GraphicsAPI::D3D11:
			CORE_ERROR("Direct3D 11 is not supported yet");
			return nullptr;
		case GraphicsAPI::D3D12:
			CORE_ERROR("Direct3D 12 is not supported yet");
			return nullptr;
		case GraphicsAPI::Vulkan:
			return new VulkanDeviceManager();
		}
		return nullptr;
	}
	
	bool DeviceManager::CreateInstance(const InstanceParameters& params)
	{
		if (m_InstanceCreated) return true;
		if(!glfwInit())
		{
			CORE_ERROR("Failed to initialize GLFW");
			return false;
		}

		// 邪门用法 把m_DeviceParameters里继承的参数替换成params的
		static_cast<InstanceParameters&>(m_DeviceParameters) = params;
		m_InstanceCreated = CreateInstanceInternal();
		return m_InstanceCreated;
	}

	bool DeviceManager::CreateWindowDeviceAndSwapChain(const DeviceParameters& params,
	                                                   const char* windowTtitle)
	{
		m_DeviceParameters = params;
		m_EnableVSync = params.vsyncEnabled;
		m_IsFullscreen = params.startFullscreen;

		// 这里init了glfw
		if(!CreateInstance(params))
		{
			CORE_ERROR("Failed to Create Instance");
			return false;
		}

		glfwSetErrorCallback(GlfwErrorCallback);
		glfwDefaultWindowHints();

		// 初始化交换链Format的hint属性
		bool foundFormat = false;
		for(const auto& info:formatInfo )
		{
			if(info.format == params.swapChainFormat)
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
		CORE_ASSERT(foundFormat, "Unsupported swap chain format : {0}", static_cast<uint32_t>(params.swapChainFormat));

		glfwWindowHint(GLFW_REFRESH_RATE, params.refreshRate);
		glfwWindowHint(GLFW_SAMPLES, params.swapChainSampleCount);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // 不使用OpenGL
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // 创建窗口时不显示
		    // 是否无边框
		if (params.startBorderless)
		{
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		}

		m_Window = glfwCreateWindow(params.backBufferWidth, params.backBufferHeight, windowTtitle,
		                            nullptr, // monitor
		                            nullptr  // share
		);

		if(m_Window == nullptr)
		{
			CORE_ERROR("Failed to create GLFW window");
			return false;
		}
		// 这一段有点没看懂 可能是为了照顾系统缩放 但是为什么没考虑全屏的？
		if (params.startFullscreen)
		{
			glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, m_DeviceParameters.backBufferWidth,
			                     m_DeviceParameters.backBufferHeight, m_DeviceParameters.refreshRate);
		}
		else
		{
			int fbWidth = 0, fbHeight = 0;
			glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
			m_DeviceParameters.backBufferWidth = fbWidth;
			m_DeviceParameters.backBufferHeight = fbHeight;
		}
		// 设置userdata
		Application* appPointer = &Application::Instance();
		glfwSetWindowUserPointer(m_Window, appPointer);

		// 设置窗口位置
		if (params.windowPosX != -1 && params.windowPosY != -1)
		{
			glfwSetWindowPos(m_Window, params.windowPosX, params.windowPosY);
		}
		// 接管Glfw回调
		glfwSetWindowPosCallback(m_Window, WindowPosCallback_GLFW);
		glfwSetWindowCloseCallback(m_Window, WindowCloseCallback_GLFW);
		glfwSetWindowRefreshCallback(m_Window, WindowRefreshCallback_GLFW);
		glfwSetWindowFocusCallback(m_Window, WindowFocusCallback_GLFW);
		glfwSetWindowIconifyCallback(m_Window, WindowIconifyCallback_GLFW);
		glfwSetKeyCallback(m_Window, KeyCallback_GLFW);
		// glfwSetCharModsCallback(m_Window, CharModsCallback_GLFW);
		glfwSetCursorPosCallback(m_Window, MousePosCallback_GLFW);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback_GLFW);
		glfwSetScrollCallback(m_Window, MouseScrollCallback_GLFW);

		// window创建好之后，surface就有了，可以创建Device和SwapChain了
		if (!CreateNvrhiDevice())
		{
			CORE_ERROR("Failed to create Device and SwapChain");
			return false;
		}
		if (!CreateSwapChain())
		{
			CORE_ERROR("Failed to create SwapChain");
			return false;
		}

		if(m_DeviceParameters.startMaximizedWindow)
		{
			glfwMaximizeWindow(m_Window);
		}
		glfwShowWindow(m_Window);
		// resize swapchain
		// reset the back buffer size state to enforce a resize event
		m_DeviceParameters.backBufferWidth = 0;
		m_DeviceParameters.backBufferHeight = 0;

		UpdateWindowSize();
		return true;
	}
	
	void DeviceManager::Render()
	{
		
	}
	void DeviceManager::UpdateWindowSize()
	{
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		if(width == 0|| height == 0)
		{
			m_WindowVisible =false;
			return;
		}
		m_WindowVisible = true;
		m_WindowFocused = glfwGetWindowAttrib(m_Window, GLFW_FOCUSED) == 1;
		if (static_cast<int>(m_DeviceParameters.backBufferWidth) != width ||
		    static_cast<int>(m_DeviceParameters.backBufferHeight) != height ||
		    m_DeviceParameters.vsyncEnabled != m_EnableVSync)
		{
			BackBufferResizing();

			m_DeviceParameters.backBufferWidth = width;
			m_DeviceParameters.backBufferHeight = height;
			m_DeviceParameters.vsyncEnabled = m_EnableVSync;

			ResizeSwapChain();
			BackBufferResized();
		}
	}

	void DeviceManager::BackBufferResizing()
	{
		// m_SwapChainFramebuffers.clear();

		// for (auto it : m_vRenderPasses)
		// {
		// 	it->BackBufferResizing();
		// }
	}

	void DeviceManager::BackBufferResized()
	{
		// for (auto it : m_vRenderPasses)
		// {
		// 	it->BackBufferResized(m_DeviceParams.backBufferWidth, m_DeviceParams.backBufferHeight,
		// 	                      m_DeviceParams.swapChainSampleCount);
		// }

		// uint32_t backBufferCount = GetBackBufferCount();
		// m_SwapChainFramebuffers.resize(backBufferCount);
		// for (uint32_t index = 0; index < backBufferCount; index++)
		// {
		// 	m_SwapChainFramebuffers[index] = GetDevice()->createFramebuffer(
		// 	    nvrhi::FramebufferDesc().addColorAttachment(GetBackBuffer(index)));
		// }
	}
} // namespace Hoshino