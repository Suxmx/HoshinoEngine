#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Renderer/TextureFormat.h"
#include "Hoshino/Renderer/RenderMessageCallback.h"
#include "Hoshino/Renderer/Device.h"
#include <nvrhi/nvrhi.h>

#ifdef HOSHINO_VULKAN
	#include <vulkan/vulkan.h>
#endif

namespace Hoshino
{
	enum class GraphicsAPI : uint8_t
	{
		D3D11 = static_cast<uint8_t>(nvrhi::GraphicsAPI::D3D11),
		D3D12 = static_cast<uint8_t>(nvrhi::GraphicsAPI::D3D12),
		Vulkan = static_cast<uint8_t>(nvrhi::GraphicsAPI::VULKAN),
	};
#pragma region Parameters
	struct InstanceParameters
	{
#ifdef HOSHINO_VULKAN
		std::vector<std::string> requiredVulkanInstanceExtensions;
		std::vector<std::string> requiredVulkanLayers;
		std::vector<std::string> optionalVulkanInstanceExtensions;
		std::vector<std::string> optionalVulkanLayers;
#endif
	};

	struct DeviceParameters
	{
		bool startMaximized = false; // ignores backbuffer width/height to be monitor size
		bool startFullscreen = false;
		bool startBorderless = false;
		bool allowModeSwitch = false;
		int windowPosX = -1; // -1 means use default placement
		int windowPosY = -1;
		uint32_t backBufferWidth = 1280;
		uint32_t backBufferHeight = 720;
		uint32_t refreshRate = 0;
		uint32_t swapChainBufferCount = 3;
		TextureFormat swapChainFormat = TextureFormat::SRGBA8_UNORM;
		uint32_t swapChainSampleCount = 1;
		uint32_t swapChainSampleQuality = 0;
		bool vsyncEnabled = false;

		RenderMsgCb* messageCallback = nullptr;
#ifdef HOSHINO_VULKAN
		std::vector<size_t> ignoredVulkanValidationMessageLocations = {0x13365b2};
#endif
	};

#pragma endregion Parameters
// Basicly from donut	
class HOSHINO_API DeviceManager
	{
	public:
		static DeviceManager* Create(GraphicsAPI api);
		void CreateInstance(const InstanceParameters& params);
		void CreateWindowDeviceAndSwapChain(const DeviceParameters& params);

	protected:
		virtual bool CreateInstanceInternal() = 0;
		virtual bool CreateDevice() = 0;
		virtual bool CreateSwapChain() = 0;
		virtual void ResizeSwapChain(uint32_t width, uint32_t height) = 0;
		virtual void DestroyDeviceAndSwapChain() = 0;
		virtual bool BeginFrame() = 0;
		virtual bool Present() = 0;
	public:
		virtual nvrhi::IDevice* GetDevice() const = 0;

	protected:
		DeviceParameters m_DeviceParameters;
		InstanceParameters m_InstanceParameters;
	};
} // namespace Hoshino