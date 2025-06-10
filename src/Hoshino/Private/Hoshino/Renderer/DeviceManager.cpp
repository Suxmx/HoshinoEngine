#include "Hoshino/Renderer/DeviceManager.h"
#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Application.h"
#include "Platform/Vulkan/VulkanDeviceManager.h"

namespace Hoshino
{
	bool DeviceManager::CreateInstance(const InstanceParameters& params)
	{
		if (m_InstanceCreated) return true;
		// 邪门用法 把m_DeviceParameters里继承的参数替换成params的
		static_cast<InstanceParameters&>(m_DeviceParameters) = params;
		m_InstanceCreated = CreateInstanceInternal();
		return m_InstanceCreated;
	}

	bool DeviceManager::CreateDevice(const DeviceParameters& params)
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

		return true;
	}

	void DeviceManager::OnResize(uint32_t width, uint32_t height)
	{
		m_DeviceParameters.backBufferWidth = width;
		m_DeviceParameters.backBufferHeight = height;
		ResizeSwapChain();
	}

	void DeviceManager::Render()
	{
		
	}

	DeviceManager* DeviceManager::Create(GraphicsAPI api, GLFWwindow* window)
	{

		switch (api)
		{
		case GraphicsAPI::D3D11:
			return nullptr;
		case GraphicsAPI::D3D12:
			return nullptr;
		case GraphicsAPI::Vulkan:
			return new VulkanDeviceManager(window);
		}
	}
	
} // namespace Hoshino