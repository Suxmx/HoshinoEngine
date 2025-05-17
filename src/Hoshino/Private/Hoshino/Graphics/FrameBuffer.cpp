#include "Hoshino/Graphics/FrameBuffer.h"
#include "Hoshino/Graphics/RenderAPI.h"

#include "Platform/OpenGL/OpenGLFrameBuffer.h"

namespace Hoshino
{
	Ref<Framebuffer> Framebuffer::Create(const FrameBufferSpec& spec)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::OpenGL:
			return CreateRef<OpenGLFrameBuffer>(spec);
		}
	}
} // namespace Hoshino