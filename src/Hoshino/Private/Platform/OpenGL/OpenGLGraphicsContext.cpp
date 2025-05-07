#include "Platform/OpenGL/OpenGLGraphicsContext.h"
#include "Hoshino/Log.h"
#include "Hoshino/Assert.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Hoshino
{
	void OpenGLGraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_ASSERT(status, "Glad Init Failed !");
		CORE_DEBUG("Glad Init Success !");
		PrintRendererInfo();
	}

	void OpenGLGraphicsContext::SwapBuffer()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
    void OpenGLGraphicsContext::PrintRendererInfo()
    {
		CORE_DEBUG("\nOpenGL Info:\n\tVendor: {0}\n\tRenderer: {1}\n\tVersion: {2}",
		           (const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER),
		           (const char*)glGetString(GL_VERSION));
	}
} // namespace Hoshino