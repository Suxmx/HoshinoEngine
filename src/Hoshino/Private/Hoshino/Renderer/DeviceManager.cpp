#include "Hoshino/Renderer/DeviceManager.h"

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
	} formatInfo [] = {
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
		glfwSetWindowUserPointer(m_Window, this);

		// 设置窗口位置
		if (params.windowPosX != -1 && params.windowPosY != -1)
		{
			glfwSetWindowPos(m_Window, params.windowPosX, params.windowPosY);
		}
		// 接管Glfw回调
	// 	glfwSetWindowPosCallback(m_Window, WindowPosCallback_GLFW);
	// 	glfwSetWindowCloseCallback(m_Window, WindowCloseCallback_GLFW);
	// 	glfwSetWindowRefreshCallback(m_Window, WindowRefreshCallback_GLFW);
	// 	glfwSetWindowFocusCallback(m_Window, WindowFocusCallback_GLFW);
	// 	glfwSetWindowIconifyCallback(m_Window, WindowIconifyCallback_GLFW);
	// 	glfwSetKeyCallback(m_Window, KeyCallback_GLFW);
	// 	glfwSetCharModsCallback(m_Window, CharModsCallback_GLFW);
	// 	glfwSetCursorPosCallback(m_Window, MousePosCallback_GLFW);
	// 	glfwSetMouseButtonCallback(m_Window, MouseButtonCallback_GLFW);
	// 	glfwSetScrollCallback(m_Window, MouseScrollCallback_GLFW);
	// 	glfwSetJoystickCallback(JoystickConnectionCallback_GLFW);
	}

} // namespace Hoshino