#include "Hoshino/Renderer/DeviceManager.h"

#include "Platform/Vulkan/VulkanDeviceManager.h"

namespace Hoshino
{
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
	
	void DeviceManager::CreateWindowDeviceAndSwapChain(const DeviceParameters& params,
												const char* windowTtitle)
	{
		m_DeviceParameters = params;
		m_EnableVSync = params.vsyncEnabled;
		m_IsFullscreen = params.startFullscreen;

		

	}

} // namespace Hoshino