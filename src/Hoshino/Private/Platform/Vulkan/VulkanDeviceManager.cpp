#include "Platform/Vulkan/VulkanDeviceManager.h"
#include "Hoshino/Renderer/RenderMessageCallback.h"
#include <cstddef>

#include <nvrhi/vulkan.h>
#include <nvrhi/validation.h>
// Define the Vulkan dynamic dispatcher - this needs to occur in exactly one cpp file in the program.
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
using string = std::string;
namespace Hoshino
{
	static std::vector<const char*> stringSetToVector(const std::unordered_set<std::string>& set)
	{
		std::vector<const char*> ret;
		for (const auto& s : set)
		{
			ret.push_back(s.c_str());
		}

		return ret;
	}

	template <typename T>
	static std::vector<T> setToVector(const std::unordered_set<T>& set)
	{
		std::vector<T> ret;
		for (const auto& s : set)
		{
			ret.push_back(s);
		}

		return ret;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(vk::DebugReportFlagsEXT flags,
	                                                          vk::DebugReportObjectTypeEXT objType,
	                                                          uint64_t obj, size_t location, int32_t code,
	                                                          const char* layerPrefix, const char* msg,
	                                                          void* userData)
	{
		CORE_WARN("[Vulkan: location=0x{0} code={1}, layerPrefix='{2}'] {3}", location, code, layerPrefix,
		          msg);
		return VK_FALSE;
	}

	bool VulkanDeviceManager::CreateInstanceInternal()
	{
		m_DynamicLoader = std::make_unique<VulkanDynamicLoader>();

		// https://zhuanlan.zhihu.com/p/534638371 用于链接扩展中的调试层函数
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
		    m_DynamicLoader->getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);
		if (!glfwVulkanSupported())
		{
			CORE_ERROR("Vulkan is not supported by GLFW.");
			return false;
		}
		// 开始创建vkInstance
		// 先处理扩展
		uint32_t glfwExtCnt = 0;
		if (m_DeviceParameters.enableDebugRuntime)
		{
			enabledExtensions.instance.insert("VK_EXT_debug_report");
			enabledExtensions.layers.insert("VK_LAYER_KHRONOS_validation");
		}
		// 添加glfw所需的
		const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCnt);
		for (int i = 0; i < glfwExtCnt; i++)
		{
			enabledExtensions.instance.insert(glfwExts[i]);
		}
		// 添加参数所需的
		for (const string& ext : m_DeviceParameters.requiredVulkanInstanceExtensions)
		{
			enabledExtensions.instance.insert(ext);
		}
		for (const string& ext : m_DeviceParameters.optionalVulkanInstanceExtensions)
		{
			optionalExtensions.instance.insert(ext);
		}

		std::unordered_set<string> requiredExts = enabledExtensions.instance;
		for (const auto& vkExt : vk::enumerateInstanceExtensionProperties())
		{
			string name = vkExt.extensionName;
			// 如果optional有可满足的顺便加入
			if (optionalExtensions.instance.find(name) != optionalExtensions.instance.end())
			{
				enabledExtensions.instance.insert(name);
			}
			requiredExts.erase(name);
		}
		// 不满足的
		if (!requiredExts.empty())
		{
			std::stringstream ss;
			ss << "Required Vulkan instance extensions not supported: ";
			for (const auto& ext : requiredExts)
			{
				ss << "\t" << ext << "\n";
			}
			CORE_ERROR("{0}", ss.str());
			return false;
		}
		std::stringstream vkInstanceExtsLogSS;
		vkInstanceExtsLogSS << "Enabled Vulkan instance extension:";
		for (const auto& ext : enabledExtensions.instance)
		{
			vkInstanceExtsLogSS << "\n\t" << ext;
		}
		CORE_INFO("{0}", vkInstanceExtsLogSS.str());
		// Layers
		for (const auto& layer : m_DeviceParameters.requiredVulkanLayers)
		{
			enabledExtensions.layers.insert(layer);
		}
		for (const auto& layer : m_DeviceParameters.optionalVulkanLayers)
		{
			optionalExtensions.layers.insert(layer);
		}
		std::unordered_set<string> requiredLayers = enabledExtensions.layers;
		for (const auto& vkLayer : vk::enumerateInstanceLayerProperties())
		{
			string name = vkLayer.layerName;
			// 如果optional有可满足的顺便加入
			if (optionalExtensions.layers.find(name) != optionalExtensions.layers.end())
			{
				enabledExtensions.layers.insert(name);
			}
			requiredLayers.erase(name);
		}
		std::stringstream vkInstanceLayersLogSS;
		if(!enabledExtensions.layers.empty())
		{
			vkInstanceLayersLogSS << "Enabled Vulkan instance layers:";
			for (const auto& layer : enabledExtensions.layers)
			{
				vkInstanceLayersLogSS << "\n\t" << layer;
			}
		}
		else
		{
			vkInstanceLayersLogSS << "No Vulkan instance layers enabled.\n";
		}
		CORE_INFO("{0}", vkInstanceLayersLogSS.str());
		// 不满足的
		if (!requiredLayers.empty())
		{
			std::stringstream ss;
			ss << "Required Vulkan instance layers not supported: ";
			for (const auto& layer : requiredLayers)
			{
				ss << "\t" << layer << "\n";
			}
			CORE_ERROR("{0}", ss.str());
			return false;
		}
		auto instanceExts = stringSetToVector(enabledExtensions.instance);
		auto instanceLayers = stringSetToVector(enabledExtensions.layers);
		auto applicationInfo = vk::ApplicationInfo();
		// 创建vkInstance
		vk::InstanceCreateInfo instanceCreateInfo = vk::InstanceCreateInfo()
		                                                .setEnabledExtensionCount(instanceExts.size())
		                                                .setPpEnabledExtensionNames(instanceExts.data())
		                                                .setEnabledLayerCount(instanceLayers.size())
		                                                .setPpEnabledLayerNames(instanceLayers.data())
		                                                .setPApplicationInfo(&applicationInfo);
		vk::Result res = vk::createInstance(&instanceCreateInfo, nullptr, &m_VkInstance);
		if (res != vk::Result::eSuccess)
		{
			CORE_ERROR("Failed to create a Vulkan instance, error code = {0}",
			           nvrhi::vulkan::resultToString(VkResult(res)));
			return false;
		}
		VULKAN_HPP_DEFAULT_DISPATCHER.init(m_VkInstance);
		return true;
	}

	bool VulkanDeviceManager::CreateNvrhiDevice()
	{
		// 运行时调试
		if (m_DeviceParameters.enableDebugRuntime)
		{
			auto info =
			    vk::DebugReportCallbackCreateInfoEXT()
			        .setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning |
			                  //   vk::DebugReportFlagBitsEXT::eInformation |
			                  vk::DebugReportFlagBitsEXT::ePerformanceWarning)
			        .setPfnCallback(vulkanDebugCallback)
			        .setPUserData(this);

			vk::Result res =
			    m_VkInstance.createDebugReportCallbackEXT(&info, nullptr, &m_VkDebugReportCallback);
			CORE_ASSERT(res == vk::Result::eSuccess,
			            "Failed to create Vulkan debug report callback, error code = {0}",
			            nvrhi::vulkan::resultToString(VkResult(res)));
		}
		// 添加参数请求的DeviceExtensions
		for (const auto& ext : m_DeviceParameters.requiredVulkanDeviceExtensions)
		{
			enabledExtensions.device.insert(ext);
		}
		for (const auto& ext : m_DeviceParameters.optionalVulkanDeviceExtensions)
		{
			optionalExtensions.device.insert(ext);
		}
		if (!CreateVkWindowsSurface())
		{
			CORE_ERROR("Failed to create Vulkan windows surface.");
			return false;
		}
		if (!PickVkPhysicalDevice())
		{
			CORE_ERROR("Failed to pick Vulkan physical device.");
			return false;
		}
		if (!FindVkQueueFamilies(m_VkPhysicalDevice))
		{
			CORE_ERROR("Failed to find a suitable Vulkan queue family index.");
			return false;
		}
		if (!CreateVkDevice())
		{
			CORE_ERROR("Failed to create Vulkan device.");
			return false;
		}
		// 创建nvrhi::IDevice
		auto vecInstanceExt = stringSetToVector(enabledExtensions.instance);
		auto vecLayers = stringSetToVector(enabledExtensions.layers);
		auto vecDeviceExt = stringSetToVector(enabledExtensions.device);
		nvrhi::vulkan::DeviceDesc deviceDesc;
		deviceDesc.errorCB = &RenderMsgCb::Instance();
		deviceDesc.instance = m_VkInstance;
		deviceDesc.physicalDevice = m_VkPhysicalDevice;
		deviceDesc.device = m_VkDevice;
		//queue
		deviceDesc.graphicsQueue = m_GraphicsQueue;
		deviceDesc.graphicsQueueIndex = m_GraphicsQueueFamilyIndex;
		if (m_DeviceParameters.enableComputeQueue)
		{
			deviceDesc.computeQueue = m_ComputeQueue;
			deviceDesc.computeQueueIndex = m_ComputeQueueFamilyIndex;
		}
		if (m_DeviceParameters.enableCopyQueue)
		{
			deviceDesc.transferQueue = m_TransferQueue;
			deviceDesc.transferQueueIndex = m_TransferQueueFamilyIndex;
		}
		deviceDesc.instanceExtensions = vecInstanceExt.data();
		deviceDesc.numInstanceExtensions = vecInstanceExt.size();
		deviceDesc.deviceExtensions = vecDeviceExt.data();
		deviceDesc.numDeviceExtensions = vecDeviceExt.size();
		deviceDesc.bufferDeviceAddressSupported = m_BufferDeviceAddressSupported;
#if HOSHINO_AFTERMATH	
		deviceDesc.aftermathEnabled = m_DeviceParams.enableAftermath;
#endif
		m_NvrhiDevice = nvrhi::vulkan::createDevice(deviceDesc);
		//TODO: validationLayer
		return true;
	}

	

	bool VulkanDeviceManager::CreateSwapChain()
	{
		DestroyVkSwapChain();
		if(!CreateVkSwapChain())
		{
			return false;
		}
		// 在Present()中用于vkQueuePresentKHR 通知GPU等待所有渲染操作完成后再呈现图像
		m_PresentSemaphores.reserve(m_DeviceParameters.maxFramesInFlight + 1);
		// 在BeginFrame()中用于vkAcquireNextImageKHR 通知GPU等待直到交换链图像可用后再开始渲染
		m_AcquireSemaphores.reserve(m_DeviceParameters.maxFramesInFlight + 1);
		for (uint32_t i = 0; i < m_DeviceParameters.maxFramesInFlight + 1; ++i)
		{
			m_PresentSemaphores.push_back(m_VkDevice.createSemaphore(vk::SemaphoreCreateInfo()));
			m_AcquireSemaphores.push_back(m_VkDevice.createSemaphore(vk::SemaphoreCreateInfo()));
		}
		return true;
	}

	void VulkanDeviceManager::ResizeSwapChain()
	{
		if (m_VkDevice)
		{
			DestroyVkSwapChain();
			CreateVkSwapChain();
		}
	}

	void VulkanDeviceManager::DestroyDeviceAndSwapChain()
	{
		DestroyVkSwapChain();
		for (auto& semaphore : m_AcquireSemaphores)
		{
			m_VkDevice.destroySemaphore(semaphore);
		}
		m_AcquireSemaphores.clear();
		for (auto& semaphore : m_PresentSemaphores)
		{
			m_VkDevice.destroySemaphore(semaphore);
		}
		m_PresentSemaphores.clear();
		m_NvrhiDevice = nullptr;

		m_DeviceName.clear();

		if (m_VkDevice)
		{
			m_VkDevice.destroy();
			m_VkDevice = nullptr;
		}

		if (m_VkSurface)
		{
			assert(m_VkInstance);
			m_VkInstance.destroySurfaceKHR(m_VkSurface);
			m_VkInstance = nullptr;
		}

		if (m_VkDebugReportCallback)
		{
			m_VkInstance.destroyDebugReportCallbackEXT(m_VkDebugReportCallback);
		}

		if (m_VkInstance)
		{
			m_VkInstance.destroy();
			m_VkInstance = nullptr;
		}
	}

	bool VulkanDeviceManager::BeginFrame()
	{
		const vk::Semaphore& acquireSemaphore = m_AcquireSemaphores[m_AcquireSemaphoreIndex];
		vk::Result res;
		int const maxAttempts = 3;
		for (int attempt = 0; attempt < maxAttempts; ++attempt)
		{
			res = m_VkDevice.acquireNextImageKHR(m_VkSwapChain, UINT64_MAX, acquireSemaphore, vk::Fence(),
			                                     &m_SwapChainIndex);
			if ((res == vk::Result::eErrorOutOfDateKHR || res == vk::Result::eSuboptimalKHR) &&
			    attempt < maxAttempts)
			{
				BackBufferResizing();
				auto surfaceCaps = m_VkPhysicalDevice.getSurfaceCapabilitiesKHR(m_VkSurface);

				m_DeviceParameters.backBufferWidth = surfaceCaps.currentExtent.width;
				m_DeviceParameters.backBufferHeight = surfaceCaps.currentExtent.height;

				ResizeSwapChain();
				BackBufferResized();
			}
			else break;
		}

		m_AcquireSemaphoreIndex = (m_AcquireSemaphoreIndex + 1) % m_AcquireSemaphores.size();
		if (res == vk::Result::eSuccess)
		{
			m_NvrhiDevice->queueWaitForSemaphore(nvrhi::CommandQueue::Graphics, acquireSemaphore, 0);
			return true;
		}
		return false;
	}

	bool VulkanDeviceManager::Present()
	{
		const vk::Semaphore& presentSemaphore = m_PresentSemaphores[m_PresentSemaphoreIndex];
		m_NvrhiDevice->queueSignalSemaphore(nvrhi::CommandQueue::Graphics,presentSemaphore, 0);
		// From Donut
		// NVRHI buffers the semaphores and signals them when something is submitted to a queue.
		// Call 'executeCommandLists' with no command lists to actually signal the semaphore.
		m_NvrhiDevice->executeCommandLists(nullptr, 0);
		vk::PresentInfoKHR presentInfo = vk::PresentInfoKHR()
		                                     .setWaitSemaphoreCount(1)
		                                     .setPWaitSemaphores(&presentSemaphore)
		                                     .setSwapchainCount(1)
		                                     .setPSwapchains(&m_VkSwapChain)
		                                     .setPImageIndices(&m_SwapChainIndex);
		// 这里实际上是CPU同步向GPU发送了Present请求，但是参数中有传递信号量
		// 在GPU处是异步的，等信号量置位之后才会present，不会阻塞CPU
		auto res = m_PresentQueue.presentKHR(&presentInfo);
		if (!(res == vk::Result::eSuccess || res == vk::Result::eErrorOutOfDateKHR ||
		      res == vk::Result::eSuboptimalKHR))
		{
			return false;
		}
		m_PresentSemaphoreIndex = (m_PresentSemaphoreIndex + 1) % m_PresentSemaphores.size();

		// 等待直到剩余渲染帧为最大帧-1（因为还要加上当前帧
		while (m_FrameInFlights.size()>=m_DeviceParameters.maxFramesInFlight)
		{
			nvrhi::EventQueryHandle query = m_FrameInFlights.front();
			m_FrameInFlights.pop();
			m_NvrhiDevice->waitEventQuery(query);

			m_QueryPool.push(query);
		}
		// 池里有就优先从池子里面取
		nvrhi::EventQueryHandle curFrameQuery;
		if(!m_QueryPool.empty())
		{
			curFrameQuery = m_QueryPool.front();
			m_QueryPool.pop();
		}
		else 
		{
			curFrameQuery = m_NvrhiDevice->createEventQuery();
		}
		
		m_NvrhiDevice->resetEventQuery(curFrameQuery);
		m_NvrhiDevice->setEventQuery(curFrameQuery, nvrhi::CommandQueue::Graphics);
		m_FrameInFlights.push(curFrameQuery);

		return true;
	}
	
	uint32_t VulkanDeviceManager::GetBackBufferCount()
	{
		return uint32_t(m_SwapChainImages.size());
	}
	
	nvrhi::ITexture* VulkanDeviceManager::GetBackBuffer(uint32_t index)
	{
		if (index < m_SwapChainImages.size()) return m_SwapChainImages[index].rhiHandle;
		return nullptr;
	}

	nvrhi::IDevice* VulkanDeviceManager::GetDevice() const
	{
		return m_NvrhiDevice;
	}
	
	nvrhi::IFramebuffer* VulkanDeviceManager::GetCurrentFramebuffer() 
	{
		return m_SwapChainFramebuffers[GetCurrentBackBufferIndex()];
	}

	bool VulkanDeviceManager::CreateVkWindowsSurface()
	{
		VkResult res = glfwCreateWindowSurface(m_VkInstance, m_Window, nullptr, (VkSurfaceKHR*)&m_VkSurface);
		if (res != VK_SUCCESS)
		{
			CORE_ERROR("Failed to create Vulkan window surface, error code = {0}",
			           nvrhi::vulkan::resultToString(res));
			return false;
		}
		return true;
	}

	bool VulkanDeviceManager::PickVkPhysicalDevice()
	{
		VkFormat requiredVkFormat =
		    nvrhi::vulkan::convertFormat((nvrhi::Format)m_DeviceParameters.swapChainFormat);
		VkExtent2D requiredExtent = {m_DeviceParameters.backBufferWidth, m_DeviceParameters.backBufferHeight};
		std::vector<vk::PhysicalDevice> allDevices = m_VkInstance.enumeratePhysicalDevices();
		std::vector<vk::PhysicalDevice> discreteDevices;   // 独显
		std::vector<vk::PhysicalDevice> integratedDevices; // 集显
		std::stringstream errSS;
		// 预先构建
		errSS << "Cannot find a Vulkan device that supports all the required extensions and properties.";
		for (int i = 0; i < allDevices.size(); i++)
		{
			vk::PhysicalDevice const& device = allDevices[i];
			vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
			vk::PhysicalDeviceFeatures deviceFeatures = device.getFeatures();
			errSS << "\n" << "Device " << i << ": " << deviceProperties.deviceName;

			bool deviceSuitable = true;
			// 检查是否满足Ext
			std::unordered_set<string> requiredExts = enabledExtensions.device;
			for (const auto& vkExt : device.enumerateDeviceExtensionProperties())
			{
				requiredExts.erase(string(vkExt.extensionName.data()));
			}
			if (!requiredExts.empty())
			{
				errSS << "\n Required extensions not supported: ";
				for (const auto& ext : requiredExts)
				{
					errSS << "\n\t" << ext;
				}
				deviceSuitable = false;
			}
			// 各向异性过滤
			if (!deviceFeatures.samplerAnisotropy)
			{
				errSS << "\n Sampler anisotropy not supported.";
				deviceSuitable = false;
			}
			// 纹理压缩
			if (!deviceFeatures.textureCompressionBC)
			{
				errSS << "\n TextureCompressionBC not supported.";
				deviceSuitable = false;
			}
			// 检查队列簇
			if (!FindVkQueueFamilies(device))
			{
				errSS << "\n Failed to find a suitable queue family.";
				deviceSuitable = false;
			}
			// 检查Window及Surface
			if (m_VkSurface)
			{
				bool surfaceSupported = device.getSurfaceSupportKHR(m_PresentQueueFamilyIndex, m_VkSurface);
				if (!surfaceSupported)
				{
					errSS << "\n Surface not supported.";
					deviceSuitable = false;
				}
				else // 检查surface是否满足swapchain所需的贴图的大小、数量、格式要求
				{
					vk::SurfaceCapabilitiesKHR surfaceCapabilities =
					    device.getSurfaceCapabilitiesKHR(m_VkSurface);
					std::vector<vk::SurfaceFormatKHR> surfaceFormats =
					    device.getSurfaceFormatsKHR(m_VkSurface);
					// swapchain贴图数量
					if (surfaceCapabilities.minImageCount > m_DeviceParameters.swapChainBufferCount ||
					    surfaceCapabilities.maxImageCount < m_DeviceParameters.swapChainBufferCount)
					{
						errSS << "\n Swapchain buffer count not supported."
						      << "\n\trequested: " << m_DeviceParameters.swapChainBufferCount
						      << ", available: " << surfaceCapabilities.minImageCount << " - "
						      << surfaceCapabilities.maxImageCount;
						deviceSuitable = false;
					}
					// swapchain大小
					if (surfaceCapabilities.minImageExtent.width > requiredExtent.width ||
					    surfaceCapabilities.minImageExtent.height > requiredExtent.height ||
					    surfaceCapabilities.maxImageExtent.width < requiredExtent.width ||
					    surfaceCapabilities.maxImageExtent.height < requiredExtent.height)
					{
						errSS << "\n Swapchain extent not supported."
						      << "\n\trequested: " << requiredExtent.width << "x" << requiredExtent.height
						      << ", available: " << surfaceCapabilities.minImageExtent.width << "x"
						      << surfaceCapabilities.minImageExtent.height << " - "
						      << surfaceCapabilities.maxImageExtent.width << "x"
						      << surfaceCapabilities.maxImageExtent.height;
						deviceSuitable = false;
					}
					// 遍历支持格式查看是否支持swapchain所需的
					bool formatSupported = false;
					for (const auto& format : surfaceFormats)
					{
						if (format.format == vk::Format(requiredVkFormat))
						{
							formatSupported = true;
							break;
						}
					}
					if (!formatSupported)
					{
						errSS << "\n Swapchain format not supported.";
						deviceSuitable = false;
					}
				}
			}
			// 不满足就不加入待选
			if (!deviceSuitable)
			{
				continue;
			}
			if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
			{
				discreteDevices.push_back(device);
			}
			else
			{
				integratedDevices.push_back(device);
			}
		}
		// 优先选独显
		if (!discreteDevices.empty())
		{
			m_VkPhysicalDevice = discreteDevices[0];
			CORE_INFO("Picked Vulkan physical discrete device: {0}",
			          m_VkPhysicalDevice.getProperties().deviceName.data());
			return true;
		}
		if (!integratedDevices.empty())
		{
			m_VkPhysicalDevice = integratedDevices[0];
			CORE_INFO("Picked Vulkan physical integrated device: {0}",
			          m_VkPhysicalDevice.getProperties().deviceName.data());
			return true;
		}
		CORE_ERROR("{0}", errSS.str());
		return false;
	}

	bool VulkanDeviceManager::FindVkQueueFamilies(const vk::PhysicalDevice& physicalDevice)
	{
		// copy from donut
		auto allQueueFamilies = physicalDevice.getQueueFamilyProperties();
		for (int i = 0; i < (int)allQueueFamilies.size(); i++)
		{
			const vk::QueueFamilyProperties& queueFamily = allQueueFamilies[i];
			if (m_GraphicsQueueFamilyIndex == -1)
			{
				if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
				{
					m_GraphicsQueueFamilyIndex = i;
				}
			}

			// 问GPT说是具有更少功能的QueueFamily相应的能力更强
			if (m_ComputeQueueFamilyIndex == -1)
			{
				if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & vk::QueueFlagBits::eCompute) &&
				    !(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
				{
					m_ComputeQueueFamilyIndex = i;
				}
			}

			if (m_TransferQueueFamilyIndex == -1)
			{
				if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) &&
				    !(queueFamily.queueFlags & vk::QueueFlagBits::eCompute) &&
				    !(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics))
				{
					m_TransferQueueFamilyIndex = i;
				}
			}

			if (m_PresentQueueFamilyIndex == -1)
			{
				if (queueFamily.queueCount > 0 &&
				    glfwGetPhysicalDevicePresentationSupport(m_VkInstance, physicalDevice, i))
				{
					m_PresentQueueFamilyIndex = i;
				}
			}
		}

		if (m_GraphicsQueueFamilyIndex == -1 || (m_PresentQueueFamilyIndex == -1) ||
		    (m_ComputeQueueFamilyIndex == -1 && m_DeviceParameters.enableComputeQueue) ||
		    (m_TransferQueueFamilyIndex == -1 && m_DeviceParameters.enableCopyQueue))
		{
			return false;
		}

		return true;
	}

	bool VulkanDeviceManager::CreateVkDevice()
	{
		// 根据之前找到的QueueFamilyIndex构建QueueCreateInfo
		std::unordered_set<int> uniqueQueueFamilies = {m_GraphicsQueueFamilyIndex, m_PresentQueueFamilyIndex};
		if (m_DeviceParameters.enableComputeQueue)
		{
			uniqueQueueFamilies.insert(m_ComputeQueueFamilyIndex);
		}
		if (m_DeviceParameters.enableCopyQueue)
		{
			uniqueQueueFamilies.insert(m_TransferQueueFamilyIndex);
		}
		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
		queueCreateInfos.reserve(uniqueQueueFamilies.size());
		float queuePriority = 1.0f;
		for (int queueFamilyIndex : uniqueQueueFamilies)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
			                                                .setQueueFamilyIndex(queueFamilyIndex)
			                                                .setQueueCount(1)
			                                                .setPQueuePriorities(&queuePriority);
			queueCreateInfos.push_back(queueCreateInfo);
		}
		// 看哪些Ext可以用
		auto allDeviceExt = m_VkPhysicalDevice.enumerateDeviceExtensionProperties();
		std::unordered_set<string> requiredExts = enabledExtensions.device;
		for (const auto& vkExt : allDeviceExt)
		{
			string name = vkExt.extensionName.data();
			// 如果optional有可满足的顺便加入
			if (optionalExtensions.device.find(name) != optionalExtensions.device.end())
			{
				enabledExtensions.device.insert(name);
			}
			requiredExts.erase(name);
		}
		// 必要的扩展不满足
		if (!requiredExts.empty())
		{
			std::stringstream ss;
			ss << "Required Vulkan device extensions not supported: ";
			for (const auto& ext : requiredExts)
			{
				ss << "\t" << ext << "\n";
			}
			CORE_ERROR("{0}", ss.str());
			return false;
		}
		bool accelStructSupported = false;
		bool rayPipelineSupported = false;
		bool rayQuerySupported = false;
		bool meshletsSupported = false;
		bool vrsSupported = false;
		bool interlockSupported = false;
		bool barycentricSupported = false;
		bool storage16BitSupported = false;
		bool synchronization2Supported = false;
		bool maintenance4Supported = false;
		bool aftermathSupported = false;
		std::stringstream deviceExtsLogSS;
		deviceExtsLogSS << "Enabled Vulkan device extensions:";
		for (const auto& ext : enabledExtensions.device)
		{
			deviceExtsLogSS << "\n\t" << ext;

			if (ext == VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME) accelStructSupported = true;
			else if (ext == VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) rayPipelineSupported = true;
			else if (ext == VK_KHR_RAY_QUERY_EXTENSION_NAME) rayQuerySupported = true;
			else if (ext == VK_NV_MESH_SHADER_EXTENSION_NAME) meshletsSupported = true;
			else if (ext == VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME) vrsSupported = true;
			else if (ext == VK_EXT_FRAGMENT_SHADER_INTERLOCK_EXTENSION_NAME) interlockSupported = true;
			else if (ext == VK_KHR_FRAGMENT_SHADER_BARYCENTRIC_EXTENSION_NAME) barycentricSupported = true;
			else if (ext == VK_KHR_16BIT_STORAGE_EXTENSION_NAME) storage16BitSupported = true;
			else if (ext == VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) synchronization2Supported = true;
			else if (ext == VK_KHR_MAINTENANCE_4_EXTENSION_NAME) maintenance4Supported = true;
			else if (ext == VK_NV_DEVICE_DIAGNOSTICS_CONFIG_EXTENSION_NAME) aftermathSupported = true;
			else if (ext == VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME)
				m_SwapChainMutableFormatSupported = true;
		}
		CORE_INFO("{0}", deviceExtsLogSS.str());
		std::stringstream deviceLayersLogSS;
		if (!enabledExtensions.layers.empty())
		{
			deviceLayersLogSS << "Enabled Vulkan device layers:";
			for (const auto& layer : enabledExtensions.layers)
			{
				deviceLayersLogSS << "\n\t" << layer;
			}
		}
		else
		{
			deviceLayersLogSS << "No Vulkan device layers enabled.";
		}
		CORE_INFO("{0}", deviceLayersLogSS.str());
		// pNext先用于Feature查询
		void* pNext = nullptr;
		// 处理各种features
		vk::PhysicalDeviceFeatures2 deviceFeatures2;
		// Determine support for Buffer Device Address, the Vulkan 1.2 way
		auto bufferDeviceAddressFeatures = vk::PhysicalDeviceBufferDeviceAddressFeatures();
		// Determine support for maintenance4
		auto maintenance4Features = vk::PhysicalDeviceMaintenance4Features();
		// Determine support for aftermath
		auto aftermathPhysicalFeatures = vk::PhysicalDeviceDiagnosticsConfigFeaturesNV();
		bufferDeviceAddressFeatures.pNext = pNext;
		pNext = &bufferDeviceAddressFeatures;
		if (maintenance4Supported)
		{
			maintenance4Features.pNext = pNext;
			pNext = &maintenance4Features;
		}
		if (aftermathSupported)
		{
			aftermathPhysicalFeatures.pNext = pNext;
			pNext = &aftermathPhysicalFeatures;
		}
		deviceFeatures2.pNext = pNext;
		// 获取一些Feature的State
		m_VkPhysicalDevice.getFeatures2(&deviceFeatures2);
		// 直接Cpy的donut
		auto accelStructFeatures =
		    vk::PhysicalDeviceAccelerationStructureFeaturesKHR().setAccelerationStructure(true);
		auto rayPipelineFeatures = vk::PhysicalDeviceRayTracingPipelineFeaturesKHR()
		                               .setRayTracingPipeline(true)
		                               .setRayTraversalPrimitiveCulling(true);
		auto rayQueryFeatures = vk::PhysicalDeviceRayQueryFeaturesKHR().setRayQuery(true);
		auto meshletFeatures =
		    vk::PhysicalDeviceMeshShaderFeaturesNV().setTaskShader(true).setMeshShader(true);
		auto interlockFeatures =
		    vk::PhysicalDeviceFragmentShaderInterlockFeaturesEXT().setFragmentShaderPixelInterlock(true);
		auto barycentricFeatures =
		    vk::PhysicalDeviceFragmentShaderBarycentricFeaturesKHR().setFragmentShaderBarycentric(true);
		auto storage16BitFeatures =
		    vk::PhysicalDevice16BitStorageFeatures().setStorageBuffer16BitAccess(true);
		auto vrsFeatures = vk::PhysicalDeviceFragmentShadingRateFeaturesKHR()
		                       .setPipelineFragmentShadingRate(true)
		                       .setPrimitiveFragmentShadingRate(true)
		                       .setAttachmentFragmentShadingRate(true);
		auto vulkan13features = vk::PhysicalDeviceVulkan13Features()
		                            .setSynchronization2(synchronization2Supported)
		                            .setMaintenance4(maintenance4Features.maintenance4);
		auto aftermathFeatures = vk::DeviceDiagnosticsConfigCreateInfoNV().setFlags(
		    vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableResourceTracking |
		    vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableShaderDebugInfo |
		    vk::DeviceDiagnosticsConfigFlagBitsNV::eEnableShaderErrorReporting);
		// Add a Vulkan 1.1 structure with default settings to make it easier for apps to modify them
		auto vulkan11features = vk::PhysicalDeviceVulkan11Features();
		auto vulkan12features = vk::PhysicalDeviceVulkan12Features()
		                            .setDescriptorIndexing(true)
		                            .setRuntimeDescriptorArray(true)
		                            .setDescriptorBindingPartiallyBound(true)
		                            .setDescriptorBindingVariableDescriptorCount(true)
		                            .setTimelineSemaphore(true)
		                            .setShaderSampledImageArrayNonUniformIndexing(true)
		                            .setBufferDeviceAddress(bufferDeviceAddressFeatures.bufferDeviceAddress);
		m_BufferDeviceAddressSupported = vulkan12features.bufferDeviceAddress;
		// 重置pNext用于Feature启用
		vk::PhysicalDeviceProperties physicalDeviceProperties = m_VkPhysicalDevice.getProperties();
		m_DeviceName = physicalDeviceProperties.deviceName.data();
		pNext = nullptr;
		if (accelStructSupported)
		{
			accelStructFeatures.pNext = pNext;
			pNext = &accelStructFeatures;
		}
		if (rayPipelineSupported)
		{
			rayPipelineFeatures.pNext = pNext;
			pNext = &rayPipelineFeatures;
		}
		if (rayQuerySupported)
		{
			rayQueryFeatures.pNext = pNext;
			pNext = &rayQueryFeatures;
		}
		if (meshletsSupported)
		{
			meshletFeatures.pNext = pNext;
			pNext = &meshletFeatures;
		}
		if (vrsSupported)
		{
			vrsFeatures.pNext = pNext;
			pNext = &vrsFeatures;
		}
		if (interlockSupported)
		{
			interlockFeatures.pNext = pNext;
			pNext = &interlockFeatures;
		}
		if (barycentricSupported)
		{
			barycentricFeatures.pNext = pNext;
			pNext = &barycentricFeatures;
		}
		if (storage16BitSupported)
		{
			storage16BitFeatures.pNext = pNext;
			pNext = &storage16BitFeatures;
		}
#ifdef HOSHINO_AFTERMATH
		if (aftermathSupported)
		{
			aftermathFeatures.pNext = pNext;
			pNext = &aftermathFeatures;
		}
#endif
		if (physicalDeviceProperties.apiVersion >= VK_API_VERSION_1_3)
		{
			vulkan13features.pNext = pNext;
			pNext = &vulkan13features;
		}
		if (physicalDeviceProperties.apiVersion < VK_API_VERSION_1_3 && maintenance4Supported)
		{
			maintenance4Features.pNext = pNext;
			pNext = &maintenance4Features;
		}
		vulkan11features.pNext = pNext;
		pNext = &vulkan11features;
		vulkan12features.pNext = pNext;
		auto deviceFeatures = vk::PhysicalDeviceFeatures()
		                          .setShaderImageGatherExtended(true)
		                          .setSamplerAnisotropy(true)
		                          .setTessellationShader(true)
		                          .setTextureCompressionBC(true)
		                          .setGeometryShader(true)
		                          .setImageCubeArray(true)
		                          .setShaderInt16(true)
		                          .setFillModeNonSolid(true)
		                          .setFragmentStoresAndAtomics(true)
		                          .setDualSrcBlend(true)
		                          .setVertexPipelineStoresAndAtomics(true);
		auto layerVec = stringSetToVector(enabledExtensions.layers);
		auto extVec = stringSetToVector(enabledExtensions.device);
		auto deviceCreateInfo = vk::DeviceCreateInfo()
		                            .setQueueCreateInfoCount(queueCreateInfos.size())
		                            .setPQueueCreateInfos(queueCreateInfos.data())
		                            .setPEnabledFeatures(&deviceFeatures)
		                            .setEnabledLayerCount(layerVec.size())
		                            .setPpEnabledLayerNames(layerVec.data())
		                            .setEnabledExtensionCount(extVec.size())
		                            .setPpEnabledExtensionNames(extVec.data())
		                            .setPNext(&vulkan12features);
		auto res = m_VkPhysicalDevice.createDevice(&deviceCreateInfo, nullptr, &m_VkDevice);
		if (res != vk::Result::eSuccess)
		{
			CORE_ERROR("Failed to create Vulkan device, error code = {0}",
			           nvrhi::vulkan::resultToString(VkResult(res)));
			return false;
		}
		// 获取各个Queue
		m_VkDevice.getQueue(m_GraphicsQueueFamilyIndex, 0, &m_GraphicsQueue);
		m_VkDevice.getQueue(m_PresentQueueFamilyIndex, 0, &m_PresentQueue);
		if(m_DeviceParameters.enableComputeQueue)
			m_VkDevice.getQueue(m_ComputeQueueFamilyIndex, 0, &m_ComputeQueue);
		if(m_DeviceParameters.enableCopyQueue)
			m_VkDevice.getQueue(m_TransferQueueFamilyIndex, 0, &m_TransferQueue);
		VULKAN_HPP_DEFAULT_DISPATCHER.init(m_VkDevice);
		CORE_INFO("Vulkan device created successfully: {0}", m_DeviceName);
		return true;
	}
	
	void VulkanDeviceManager::DestroyVkSwapChain()
	{
		if (m_VkDevice)
		{
			m_VkDevice.waitIdle();
		}

		if (m_VkSwapChain)
		{
			m_VkDevice.destroySwapchainKHR(m_VkSwapChain);
			m_VkSwapChain = nullptr;
		}

		m_SwapChainImages.clear();
	}

	bool VulkanDeviceManager::CreateVkSwapChain()
	{
		m_SwapChainFormat = {
		    vk::Format(nvrhi::vulkan::convertFormat((nvrhi::Format)m_DeviceParameters.swapChainFormat)),
		    vk::ColorSpaceKHR::eSrgbNonlinear};
		vk::Extent2D swapChainExtent = {m_DeviceParameters.backBufferWidth,
		                                m_DeviceParameters.backBufferHeight};
		std::unordered_set<uint32_t> uniqueQueues = {uint32_t(m_GraphicsQueueFamilyIndex),
		                                             uint32_t(m_PresentQueueFamilyIndex)};
		std::vector<uint32_t> queueFamilyIndices = setToVector(uniqueQueues);
		const bool enableSwapChainSharing = queueFamilyIndices.size() > 1;
		// 防止validation layer报错
		vk::SurfaceCapabilitiesKHR surfaceCapabilities = m_VkPhysicalDevice.getSurfaceCapabilitiesKHR(m_VkSurface);
		auto desc =
		    vk::SwapchainCreateInfoKHR()
		        .setSurface(m_VkSurface)
		        .setMinImageCount(m_DeviceParameters.swapChainBufferCount)
		        .setImageFormat(m_SwapChainFormat.format)
		        .setImageColorSpace(m_SwapChainFormat.colorSpace)
		        .setImageExtent(swapChainExtent)
		        .setImageArrayLayers(1)
		        .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment |
		                       vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled)
		        .setImageSharingMode(enableSwapChainSharing ? vk::SharingMode::eConcurrent
		                                                    : vk::SharingMode::eExclusive)
		        .setFlags(m_SwapChainMutableFormatSupported ? vk::SwapchainCreateFlagBitsKHR::eMutableFormat
		                                                    : vk::SwapchainCreateFlagBitsKHR(0))
		        .setQueueFamilyIndexCount(enableSwapChainSharing ? uint32_t(queueFamilyIndices.size()) : 0)
		        .setPQueueFamilyIndices(enableSwapChainSharing ? queueFamilyIndices.data() : nullptr)
		        .setPreTransform(vk::SurfaceTransformFlagBitsKHR::eIdentity)
		        .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
		        .setPresentMode(m_DeviceParameters.vsyncEnabled ? vk::PresentModeKHR::eFifo
		                                                        : vk::PresentModeKHR::eImmediate)
		        .setClipped(true)
		        .setOldSwapchain(nullptr);
		// 可变格式交换链 针对常见的RGBA/BGRA格式创建"格式对"
		std::vector<vk::Format> imageFormats = {m_SwapChainFormat.format};
		switch (m_SwapChainFormat.format)
		{
		case vk::Format::eR8G8B8A8Unorm:
			imageFormats.push_back(vk::Format::eR8G8B8A8Srgb);
			break;
		case vk::Format::eR8G8B8A8Srgb:
			imageFormats.push_back(vk::Format::eR8G8B8A8Unorm);
			break;
		case vk::Format::eB8G8R8A8Unorm:
			imageFormats.push_back(vk::Format::eB8G8R8A8Srgb);
			break;
		case vk::Format::eB8G8R8A8Srgb:
			imageFormats.push_back(vk::Format::eB8G8R8A8Unorm);
			break;
		default:
			break;
		}

		auto imageFormatListCreateInfo = vk::ImageFormatListCreateInfo().setViewFormats(imageFormats);

		if (m_SwapChainMutableFormatSupported) desc.pNext = &imageFormatListCreateInfo;

		const vk::Result res = m_VkDevice.createSwapchainKHR(&desc, nullptr, &m_VkSwapChain);
		if (res != vk::Result::eSuccess)
		{
			CORE_ERROR("Failed to create Vulkan swap chain, error code = {0}",
			           nvrhi::vulkan::resultToString(VkResult(res)));
			return false;
		}
		// retrieve swap chain images
		auto images = m_VkDevice.getSwapchainImagesKHR(m_VkSwapChain);
		for (auto image : images)
		{
			SwapChainImage sci;
			sci.image = image;

			nvrhi::TextureDesc textureDesc;
			textureDesc.width = m_DeviceParameters.backBufferWidth;
			textureDesc.height = m_DeviceParameters.backBufferHeight;
			textureDesc.format = (nvrhi::Format)m_DeviceParameters.swapChainFormat;
			textureDesc.debugName = "Swap chain image";
			textureDesc.initialState = nvrhi::ResourceStates::Present;
			textureDesc.keepInitialState = true;
			textureDesc.isRenderTarget = true;

			sci.rhiHandle = m_NvrhiDevice->createHandleForNativeTexture(
			    nvrhi::ObjectTypes::VK_Image, nvrhi::Object(sci.image), textureDesc);
			m_SwapChainImages.push_back(sci);
		}

		m_SwapChainIndex = 0;
		return true;
	}

	uint32_t VulkanDeviceManager::GetCurrentBackBufferIndex()
	{
		return m_SwapChainIndex;
	}
} // namespace Hoshino