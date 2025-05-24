#include "Hoshino/Graphics/RenderAPI.h"
#include "Hoshino/Graphics/Texture.h"

#include "Platform/OpenGL/OpenGLTexture.h"
namespace Hoshino
{
	Ref<Texture> Texture::Create(const std::string& path)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::OpenGL:
			return CreateRef<OpenGLTexture>(path);
		}
		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Texture> Texture::Create(const TextureSpec& spec)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::OpenGL:
			return CreateRef<OpenGLTexture>(spec);
		}
		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
} // namespace Hoshino