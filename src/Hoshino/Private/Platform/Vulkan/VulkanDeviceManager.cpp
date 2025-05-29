#include "Platform/Vulkan/VulkanDeviceManager.h"
#include <cstddef>

namespace Hoshino {
    bool VulkanDeviceManager::CreateInstanceInternal()
    {
        return VK_FALSE;
    }
    
    bool VulkanDeviceManager::CreateDevice()
    {
        return VK_FALSE;
    }
    
    bool VulkanDeviceManager::CreateSwapChain()
    {
        return VK_FALSE;
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
}