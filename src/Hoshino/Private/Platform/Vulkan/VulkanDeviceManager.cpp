#include "Platform/Vulkan/VulkanDeviceManager.h"
#include <cstddef>

#include <nvrhi/vulkan.h>
#include <nvrhi/validation.h>
// Define the Vulkan dynamic dispatcher - this needs to occur in exactly one cpp file in the program.
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
using string = std::string;
namespace Hoshino {
	static std::vector<const char*> stringSetToVector(const std::unordered_set<std::string>& set)
	{
		std::vector<const char*> ret;
		for (const auto& s : set)
		{
			ret.push_back(s.c_str());
		}

		return ret;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(vk::DebugReportFlagsEXT flags,
	                                                          vk::DebugReportObjectTypeEXT objType,
	                                                          uint64_t obj, size_t location, int32_t code,
	                                                          const char* layerPrefix, const char* msg,
	                                                          void* userData)
	{
		CORE_WARN("[Vulkan: location=0x{0} code={1}, layerPrefix='{2}'] {3}", location, code,
		                    layerPrefix, msg);
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

		std::unordered_set<string> requiredExts= enabledExtensions.instance;
		for (const auto& vkExt : vk::enumerateInstanceExtensionProperties())
		{
            // 如果optional有可满足的顺便加入
            if(optionalExtensions.instance.find(vkExt.extensionName) != optionalExtensions.instance.end())
            {
                enabledExtensions.instance.insert(vkExt.extensionName);
            }
            requiredExts.erase(vkExt.extensionName);
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
		vkInstanceExtsLogSS << "Enabled Vulkan instance extension:\n";
		for (const auto& ext : enabledExtensions.instance)
		{
			vkInstanceExtsLogSS << "\t" << ext << "\n";
		}
		CORE_INFO("{0}", vkInstanceExtsLogSS.str());
		// Layers
        for(const auto& layer : m_DeviceParameters.requiredVulkanLayers)
        {
            enabledExtensions.layers.insert(layer);
        }
        for(const auto& layer : m_DeviceParameters.optionalVulkanLayers)
        {
            optionalExtensions.layers.insert(layer);
        }
		std::unordered_set<string> requiredLayers = enabledExtensions.layers;
		for (const auto& vkLayer : vk::enumerateInstanceLayerProperties())
		{
			// 如果optional有可满足的顺便加入
			if (optionalExtensions.layers.find(vkLayer.layerName) != optionalExtensions.layers.end())
			{
				enabledExtensions.layers.insert(vkLayer.layerName);
			}
			requiredLayers.erase(vkLayer.layerName);
		}
		std::stringstream vkInstanceLayersLogSS;
		vkInstanceLayersLogSS << "Enabled Vulkan instance layers:\n";
		for (const auto& layer : enabledExtensions.layers)
		{
			vkInstanceLayersLogSS << "\t" << layer << "\n";
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
        vk::Result res = vk::createInstance(&instanceCreateInfo,nullptr,&m_VkInstance);
        if (res != vk::Result::eSuccess)
        {
			CORE_ERROR("Failed to create a Vulkan instance, error code = {0}",
			           nvrhi::vulkan::resultToString(VkResult(res)));
			return false;
		}                                    
		return true;
	}
    
    bool VulkanDeviceManager::CreateNvrhiDevice()
    {
        // 运行时调试
        if(m_DeviceParameters.enableDebugRuntime)
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
		for(const auto& ext : m_DeviceParameters.requiredVulkanDeviceExtensions)
		{
			enabledExtensions.device.insert(ext);
		}
		for(const auto& ext : m_DeviceParameters.optionalVulkanDeviceExtensions)
		{
			optionalExtensions.device.insert(ext);
		}
		if(!CreateVkWindowsSurface())
		{
			CORE_ERROR("Failed to create Vulkan windows surface.");
			return false;
		}
		if(!PickVkPhysicalDevice())
		{
			CORE_ERROR("Failed to pick Vulkan physical device.");
			return false;
		}
		if (!FindVkQueueFamilies())
		{
			CORE_ERROR("Failed to find a suitable Vulkan queue family index.");
			return false;
		}
		if(!CreateVkDevice())
		{
			CORE_ERROR("Failed to create Vulkan device.");
			return false;
		}
		// 创建nvrhi::IDevice
		

		return true;
	}
    
    bool VulkanDeviceManager::CreateSwapChain()
    {
		return false;
	}
    
    void VulkanDeviceManager::ResizeSwapChain(uint32_t width, uint32_t height)
    {
        
    }
    
    void VulkanDeviceManager::DestroyDeviceAndSwapChain()
    {
        
    }
    
    bool VulkanDeviceManager::BeginFrame()
    {
        return VK_FALSE;
    }
    
    bool VulkanDeviceManager::Present()
    {
        return VK_FALSE;
    }
    
    nvrhi::IDevice* VulkanDeviceManager::GetDevice() const 
    {
        return nullptr;
    }

	bool VulkanDeviceManager::CreateVkWindowsSurface() {
		VkResult res = glfwCreateWindowSurface(m_VkInstance, m_Window, nullptr, (VkSurfaceKHR*)&m_VkSurface);
		if (res != VK_SUCCESS)
		{
			CORE_ERROR("Failed to create Vulkan window surface, error code = {0}",
			           nvrhi::vulkan::resultToString(res));
			return false;
		}
		return true;
	}

	bool VulkanDeviceManager::PickVkPhysicalDevice() {
		VkFormat requiredVkFormat =
		    nvrhi::vulkan::convertFormat((nvrhi::Format)m_DeviceParameters.swapChainFormat);
	}

	bool VulkanDeviceManager::FindVkQueueFamilies() {}

	bool VulkanDeviceManager::CreateVkDevice() {}

}