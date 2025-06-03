#pragma once
#include "Hoshino/Renderer/DeviceManager.h"
#ifndef VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
	#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#endif
#include <vulkan/vulkan.hpp>

#if VK_HEADER_VERSION >= 301
typedef vk::detail::DynamicLoader VulkanDynamicLoader;
#else
typedef vk::DynamicLoader VulkanDynamicLoader;
#endif
namespace Hoshino
{

	class HOSHINO_API VulkanDeviceManager : public DeviceManager
	{
	protected:
		virtual bool CreateInstanceInternal() override;
		virtual bool CreateNvrhiDevice() override;
		virtual bool CreateSwapChain() override;
		virtual void ResizeSwapChain(uint32_t width, uint32_t height) override;
		virtual void DestroyDeviceAndSwapChain() override;
		virtual bool BeginFrame() override;
		virtual bool Present() override;

	public:
		virtual nvrhi::IDevice* GetDevice() const override;

	//Funcs
	private:
		bool CreateVkWindowsSurface();
		bool PickVkPhysicalDevice();
		bool FindVkQueueFamilies(const vk::PhysicalDevice& physicalDevice);
		bool CreateVkDevice();
		void DestroyVkSwapChain();
		// Properties
	private:
		bool m_BufferDeviceAddressSupported ;
		bool m_SwapChainMutableFormatSupported;
		uint32_t m_SwapChainIndex = uint32_t(-1);

		vk::Instance m_VkInstance;
		vk::SurfaceKHR m_VkSurface;
		vk::PhysicalDevice m_VkPhysicalDevice;
		vk::Device m_VkDevice;
		vk::DebugReportCallbackEXT m_VkDebugReportCallback;
		std::unique_ptr<VulkanDynamicLoader> m_DynamicLoader;
		int m_GraphicsQueueFamilyIndex = 0;
		int m_PresentQueueFamilyIndex = 0;
		int m_ComputeQueueFamilyIndex = 0;
		int m_TransferQueueFamilyIndex = 0;
		vk::Queue m_GraphicsQueue;
		vk::Queue m_PresentQueue;
		vk::Queue m_ComputeQueue;
		vk::Queue m_TransferQueue;
		std::string m_DeviceName;
		// SwapChain
		vk::SwapchainKHR m_VkSwapChain;
		struct SwapChainImage
		{
			vk::Image image;
			nvrhi::TextureHandle rhiHandle;
		};

		std::vector<SwapChainImage> m_SwapChainImages;
		vk::SurfaceFormatKHR m_SwapChainFormat;
		std::vector<vk::Semaphore> m_AcquireSemaphores;
		std::vector<vk::Semaphore> m_PresentSemaphores;
#pragma region Extensions
		struct VulkanExtensionSet
		{
			std::unordered_set<std::string> instance;
			std::unordered_set<std::string> layers;
			std::unordered_set<std::string> device;
		};
		// minimal set of required extensions
		VulkanExtensionSet enabledExtensions = {
		    // instance
		    {VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME},
		    // layers
		    {},
		    // device
		    {VK_KHR_MAINTENANCE1_EXTENSION_NAME, VK_KHR_SWAPCHAIN_EXTENSION_NAME},
		};

		// optional extensions
		VulkanExtensionSet optionalExtensions = {
		    // instance
		    {
		        // 提供增强的调试功能,允许对对象添加名称标签,提供详细的调试消息回调
		        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		        // 提供特殊的采样器，可在纹理过滤时计算最小值/最大值,对HDR渲染和某些后处理效果很有用
		        VK_EXT_SAMPLER_FILTER_MINMAX_EXTENSION_NAME,
		    },
		    // layers
		    {},
		    // device
		    {
		        // 可以在RenderDoc中增加调试标记
		        VK_EXT_DEBUG_MARKER_EXTENSION_NAME,
		        // 支持动态描述符索引 使绑定大型纹理 / 资源数组更高效
		        VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		        // 允许在着色器中直接获取缓冲区地址 光线追踪和高级计算着色器的关键功能
		        VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
		        // 可变速率着色
		        VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME,
		        VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,
		        // 增强的内存管理
		        VK_KHR_MAINTENANCE_4_EXTENSION_NAME,
		        // 允许交换链图像有多种可能的格式 增强 HDR 和色彩空间支持
		        VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
		        VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
		        // 提供更简洁的同步 API
		        VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
		        // NVIDIA 的网格着色器扩展 替代传统顶点、几何着色器管线 提供更灵活的几何处理
		        VK_NV_MESH_SHADER_EXTENSION_NAME,
#if HOSHINO_AFTERMATH
		        VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME,
		        VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME,
#endif
		    },
		};
#pragma endregion Extensions
	};
} // namespace Hoshino