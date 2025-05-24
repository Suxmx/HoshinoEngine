#include "Platform/OpenGL/OpenGLFrameBuffer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/Utils.h"

#include <glad/glad.h>

namespace Hoshino
{

	static bool IsDepthAttachment(const FrameBufferTextureSpec& spec)
	{
		return spec.Format == TextureFormat::Depth24_Stencil8 || spec.Format ==
		       TextureFormat::Depth;
	}

	static Ref<Texture> GenTexture(const FrameBufferTextureSpec& spec, int attachment, float width,
	                               float height)
	{
		TextureSpec textureSpec(spec.Format, width, height);
		Ref<Texture> texture = Texture::Create(textureSpec);
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->GetRendererID(), 0);
		return texture;
	}

	// // 先不抽象缓冲区
	// static uint32_t GenRenderBuffer(const FrameBufferTextureSpec& spec, int attachment, float width,
	//                                 float height)
	// {
	// 	uint32_t id = 0;
	// 	glGenRenderbuffers(1, &id);
	// 	glBindRenderbuffer(GL_RENDERBUFFER, id);
	// 	glRenderbufferStorage(GL_RENDERBUFFER, Utils::GetGLInternalFoamat(spec.Format), width, height);
	// 	glBindRenderbuffer(GL_RENDERBUFFER, id);
	// 	return id;
	// }

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

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x,int y)
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
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::GetGLDataFormat(spec.Format),
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
	
	Ref<Texture> OpenGLFrameBuffer::GetColorAttachmentTexture(uint32_t index) const 
	{
		return m_ColorAttachmentTextures[index];
	}

	//TODO: 临时代码 后续支持RBO
	void OpenGLFrameBuffer::Reinit()
	{
		// 如果原先已经初始化过了就先清理
		if (m_RendererID)
		{
			Delete();
		}
		m_Width = m_Spec.Width;
		m_Height = m_Spec.Height;
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
			}
			int attachment;
			if (isDepth)
			{
				if (texSpec.Format == TextureFormat::Depth24_Stencil8)
				{
					attachment = GL_DEPTH_STENCIL_ATTACHMENT;
				}
				else
				{
					attachment = GL_DEPTH_ATTACHMENT;
				}
			}
			else
			{
				attachment = m_AttachTextureIndex + GL_COLOR_ATTACHMENT0;
			}
			if(texSpec.UseRenderBuffer)
			{
				CORE_ASSERT(false, "RenderBuffer not supported yet");
				// uint32_t renderBuffer = GenRenderBuffer(texSpec, attachment, m_Width, m_Height);
				// glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderBuffer);
			}
			else
			{
				Ref<Texture> texture  =
				    GenTexture(texSpec, attachment, m_Width, m_Height);

				if (isDepth)
				{
					m_DepthAttachmentTexture  = texture;
					m_DepthAttachment = texture->GetRendererID();
					m_DepthAttachmentSpec = texSpec;
				}
				else
				{
					m_ColorAttachments.push_back(texture->GetRendererID());
					m_ColorAttachmentTextures.push_back(texture);
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
		m_ColorAttachmentTextures.clear();
		m_DepthAttachment = 0;
        m_AttachTextureIndex = -1;
        m_ColorAttachmentSpecifications.clear();
        m_DepthAttachmentSpec = FrameBufferTextureSpec();
        m_RendererID = 0;

	}
} // namespace Hoshino