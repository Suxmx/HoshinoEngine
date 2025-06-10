#include "HoshinoMin.h"

namespace Hoshino
{
	class VulkanSwapChain
	{
	public:
		VulkanSwapChain();
        void BeginFrame();
		void Present();

		void OnResize(uint32_t width, uint32_t height);
	};
} // namespace Hoshino