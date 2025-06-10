#include "HoshinoMin.h"
#include "Hoshino/Event/Event.h"
#include "Hoshino/Renderer/DeviceManager.h"
// #include <nvrhi/vulkan.h>
namespace Hoshino
{
	class VulkanSwapChain;

	struct WindowSpecification
	{
		std::string Title = "Hoshino";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Decorated = true;
		bool Fullscreen = false;
		bool VSync = true;
	};

	class HOSHINO_API Window
	{
		using EventCallbackFn = std::function<void(Event&)>;
		friend class DeviceManager;

	public:
		Window(const WindowSpecification& windowSpec) : m_Specification(windowSpec) {}
		virtual ~Window() = default;
		virtual unsigned int GetWidth()
		{
			return m_Data.Width;
		}
		virtual unsigned int GetHeight()
		{
			return m_Data.Height;
		}
		virtual void OnUpdate() {}
		// Window
		void Init();
		void BeginFrame();
		void Present();
		virtual void SetEventCallbackFn(const EventCallbackFn& fn)
		{
			m_Data.EventCallback = fn;
		}
		virtual void* GetNativeWindow()
		{
			return m_GlfwWindow;
		}
		void UpdateWindowSize();
		DeviceManager* GetDeviceManager()
		{
			return m_DeviceMgr.get();
		}
		// Create
		static Scope<Window> Create(const WindowSpecification& props = WindowSpecification());

	protected:
		Scope<DeviceManager> m_DeviceMgr = nullptr;
		Ref<VulkanSwapChain> m_SwapChain = nullptr;

		WindowSpecification m_Specification;
		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;

			bool WindowVisible = true;
			bool WindowFocused = true;
			bool VsyncEnabled = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
		GLFWwindow* m_GlfwWindow = nullptr;

	private:
		void InitGlfwCallbacks();
	};
} // namespace Hoshino