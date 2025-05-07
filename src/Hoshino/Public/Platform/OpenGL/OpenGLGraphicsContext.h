#pragma once
#include "Hoshino/Graphics/GraphicsContext.h"

struct GLFWwindow;

namespace Hoshino
{
	class HOSHINO_API OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		OpenGLGraphicsContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
		{
			Init();
		}
		~OpenGLGraphicsContext() {}

	public:
		void Init() override;
		void SwapBuffer() override;
		static void PrintRendererInfo();

	private:
		GLFWwindow* m_WindowHandle;
	};
} // namespace Hoshino