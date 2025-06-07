#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Renderer/TextureFormat.h"
#include "Hoshino/Renderer/RenderMessageCallback.h"
#include "Hoshino/Renderer/Device.h"
#include <nvrhi/nvrhi.h>

#ifdef HOSHINO_VULKAN
	#include <vulkan/vulkan.h>
#endif

#include <GLFW/glfw3.h>

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
		bool enableDebugRuntime = false; // 是否启用调试运行时
#ifdef HOSHINO_VULKAN
		std::vector<std::string> requiredVulkanInstanceExtensions;
		std::vector<std::string> requiredVulkanLayers;
		std::vector<std::string> optionalVulkanInstanceExtensions;
		std::vector<std::string> optionalVulkanLayers;
#endif
	};

	struct DeviceParameters : InstanceParameters
	{
		bool startMaximizedWindow = false; // 忽略backbuffer属性，直接最大化窗口
		bool startFullscreen = false; // 全屏模式，以显示屏为单位
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
		uint32_t maxFramesInFlight = 2; // CPU向GPU最多预先提交的帧数（多了就等待present之后再提交）

		RenderMsgCb* messageCallback = nullptr;
#ifdef HOSHINO_VULKAN
		std::vector<std::string> requiredVulkanDeviceExtensions;
		std::vector<std::string> optionalVulkanDeviceExtensions;
		std::vector<size_t> ignoredVulkanValidationMessageLocations = {0x13365b2};
		bool enableComputeQueue;
		bool enableCopyQueue;
#endif
	};

#pragma endregion Parameters
// Basicly from donut	
class HOSHINO_API DeviceManager
	{
	public:
		static DeviceManager* Create(GraphicsAPI api);
		bool CreateInstance(const InstanceParameters& params);
		bool CreateWindowDeviceAndSwapChain(const DeviceParameters& params,
		                                    const char* windowTtitle = "HoshinoEngine");

	protected:
		virtual bool CreateInstanceInternal() = 0;
		virtual bool CreateNvrhiDevice() = 0;
		virtual bool CreateSwapChain() = 0;
		virtual void ResizeSwapChain() = 0;
		virtual void DestroyDeviceAndSwapChain() = 0;
		virtual bool BeginFrame() = 0;
		virtual bool Present() = 0;

		void BackBufferResizing();
		void BackBufferResized();

		void UpdateWindowSize();
		void Render();
		

	public:
		virtual nvrhi::IDevice* GetDevice() const = 0;

	protected:
		DeviceParameters m_DeviceParameters;

		// Actual Properties
		bool m_EnableVSync = false;
		bool m_IsFullscreen = false;
		bool m_InstanceCreated = false;
		bool m_WindowVisible;
		bool m_WindowFocused;

		GLFWwindow* m_Window = nullptr;
		std::vector<nvrhi::FramebufferHandle> m_SwapChainFramebuffers;
		std::queue<nvrhi::EventQueryHandle> m_FrameInFlights; // CPU已提交但是仍在渲染的帧
		std::queue<nvrhi::EventQueryHandle> m_QueryPool;

		friend class Application;
	};
} // namespace Hoshino