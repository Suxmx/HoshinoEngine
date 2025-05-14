#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>
#include <glad/glad.h>
namespace Hoshino
{
	static GLenum TextureFormatToGL(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::RGBA:
			return GL_RGBA;
		default:
			return 0;
		}
	}

	OpenGLTexture::OpenGLTexture(const std::string& path) : Texture(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		auto data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			CORE_ERROR("Failed to load texture: {0}", path);
			return;
		}
		m_Width = width;
		m_Height = height;
		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		glCreateTextures(GL_TEXTURE_2D,1,&m_RendererID);

		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
		glTexParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTexture::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
} // namespace Hoshino