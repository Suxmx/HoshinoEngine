#pragma once
#include "Hoshino/Renderer/DeviceManager.h"
#include <vulkan/vulkan.hpp>
namespace Hoshino
{
	class HOSHINO_API VulkanDeviceManager : public DeviceManager
	{
	protected:
		virtual bool CreateInstanceInternal() override;
		virtual bool CreateDevice() override;
		virtual bool CreateSwapChain() override;
		virtual void ResizeSwapChain(uint32_t width, uint32_t height) override;
		virtual void DestroyDeviceAndSwapChain() override;
		virtual bool BeginFrame() override;
		virtual bool Present() override;

	public:
		virtual nvrhi::IDevice* GetDevice() const override;

	private:
		vk::Instance m_VkInstance;
		vk::PhysicalDevice m_VkPhysicalDevice;
		vk::Device m_VkDevice;
	};
} // namespace Hoshino