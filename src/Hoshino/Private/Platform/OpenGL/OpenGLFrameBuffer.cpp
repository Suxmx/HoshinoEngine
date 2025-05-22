#include "Platform/OpenGL/OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Hoshino
{

	static bool IsDepthAttachment(const FrameBufferTextureSpec& spec)
	{
		return spec.Format == FrameBufferTextureFormat::Depth24_Stencil8;
	}

	static GLenum GetGLInternalFoamat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::None:
			return GL_NONE;
		case FrameBufferTextureFormat::RGBA:
			return GL_RGBA8;
		case FrameBufferTextureFormat::RGB:
			return GL_RGB8;
		case FrameBufferTextureFormat::Depth24_Stencil8:
			return GL_DEPTH24_STENCIL8;
		}
		CORE_ASSERT(false, "Unknown FrameBufferTextureFormat");
        return GL_NONE; // Just to avoid compiler warning


        
	}

	static GLenum GetGLDataFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case FrameBufferTextureFormat::None:
			return GL_NONE;
		case FrameBufferTextureFormat::RGBA:
			return GL_RGBA;
		case FrameBufferTextureFormat::RGB:
			return GL_RGB;
		case FrameBufferTextureFormat::Depth24_Stencil8:
			return GL_DEPTH_STENCIL;
		}
		CORE_ASSERT(false, "Unknown FrameBufferTextureFormat");
		return GL_NONE; // Just to avoid compiler warning
	}

	static uint32_t GenAttachment(const FrameBufferTextureSpec& spec, int attachment, float width,
	                              float height)
	{
		uint32_t id = 0;
		if (spec.UseRenderBuffer)
		{
			glGenRenderbuffers(1, &id);
			glBindRenderbuffer(GL_RENDERBUFFER, id);
			glRenderbufferStorage(GL_RENDERBUFFER, GetGLInternalFoamat(spec.Format), width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, id);
		}
		else
		{
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexImage2D(GL_TEXTURE_2D, 0, GetGLInternalFoamat(spec.Format), width, height, 0,
			             GetGLDataFormat(spec.Format), GL_UNSIGNED_BYTE, nullptr);

			// Temp 需要用spec里面的描述替代
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, id, 0);
		}
		return id;
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec) : Framebuffer(spec) {
        Reinit();
    }

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		Delete();
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		m_Width = width;
		m_Height = height;
		Reinit();
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
		CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(),
		            "Index out of bounds! FrameBuffer has only {0} color attachments",
		            m_ColorAttachments.size());

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, GetGLDataFormat(spec.Format),
                        GL_INT, &value);
	}

	uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID(uint32_t index) const
	{
		CORE_ASSERT(index < m_ColorAttachments.size(),
		            "Index out of bounds! FrameBuffer has only {0} color attachments",
		            m_ColorAttachments.size());
		return m_ColorAttachments[index];
	}

	const FrameBufferSpec& OpenGLFrameBuffer::GetSpecification() const
	{
		return m_Spec;
	}

	void OpenGLFrameBuffer::Reinit()
	{
		// 如果原先已经初始化过了就先清理
		if (m_RendererID)
		{
			Delete();
		}
		glGenFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		for (const auto& texSpec : m_Spec.AttachmentsSpc.Attachments)
		{
			bool isDepth = IsDepthAttachment(texSpec);
			if (isDepth)
			{
				if (m_DepthAttachment)
				{
					CORE_ERROR("Can't add two or more depth attachments to a framebuffer");
					continue;
				}
			}
			else
			{
				if (!texSpec.UseRenderBuffer) m_AttachTextureIndex++;
				int attachment =
				    isDepth ? GL_DEPTH_STENCIL_ATTACHMENT : m_AttachTextureIndex + GL_COLOR_ATTACHMENT0;
				uint32_t attachmentId = GenAttachment(texSpec, attachment, m_Width, m_Height);
				if (isDepth)
				{
					m_DepthAttachment = attachmentId;
                    m_DepthAttachmentSpec = texSpec;
				}
				else
				{
					m_ColorAttachments.push_back(attachmentId);
                    m_ColorAttachmentSpecifications.push_back(texSpec);
				}
			}
		}
		if (m_ColorAttachments.size() > 1)
		{
			CORE_ASSERT(m_ColorAttachments.size() <= 4,
			            "OpenGL only supports a maximum of {0} color attachments!", 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
			                      GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			glDrawBuffer(GL_NONE);
		}

		CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
		            "Framebuffer not complete!");
	}
    
    void OpenGLFrameBuffer::Delete()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);

		m_ColorAttachments.clear();
		m_DepthAttachment = 0;
        m_AttachTextureIndex = -1;
        m_ColorAttachmentSpecifications.clear();
        m_DepthAttachmentSpec = FrameBufferTextureSpec();
        m_RendererID = 0;

	}
} // namespace Hoshino