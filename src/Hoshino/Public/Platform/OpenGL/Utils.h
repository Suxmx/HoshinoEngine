#include "HoshinoMin.h"
#include "Hoshino/Graphics/Texture.h"

#include <glad/glad.h>

namespace Hoshino::Utils
{
	static GLenum GetGLInternalFoamat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::None:
			return GL_NONE;
		case TextureFormat::RGBA:
			return GL_RGBA8;
		case TextureFormat::RGB:
			return GL_RGB8;
		case TextureFormat::Depth24_Stencil8:
			return GL_DEPTH24_STENCIL8;
		case TextureFormat::Depth:
			return GL_DEPTH_COMPONENT24;
		}
		CORE_ASSERT(false, "Unknown TextureFormat");
		return GL_NONE; // Just to avoid compiler warning
	}

	static GLenum GetGLDataFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::None:
			return GL_NONE;
		case TextureFormat::RGBA:
			return GL_RGBA;
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::Depth24_Stencil8:
			return GL_DEPTH_STENCIL;
		case TextureFormat::Depth:
			return GL_DEPTH_COMPONENT;
		}
		CORE_ASSERT(false, "Unknown TextureFormat");
		return GL_NONE; // Just to avoid compiler warning
	}
} // namespace Hoshino