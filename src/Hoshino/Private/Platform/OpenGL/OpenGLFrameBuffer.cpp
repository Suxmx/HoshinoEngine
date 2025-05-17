#include "Platform/OpenGL/OpenGLFrameBuffer.h"

#include <glad/glad.h>

namespace Hoshino {
    OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpec& spec) : Framebuffer(spec)
    {
        
    }
    
    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        Delete();
    }
    
    void OpenGLFrameBuffer::Bind()
    {
        
    }
    
    void OpenGLFrameBuffer::Unbind()
    {
        
    }
    
    void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
    {
        
    }
    
    int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
    {
        
    }
    
    void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
    {
        
    }
    
    uint32_t OpenGLFrameBuffer::GetColorAttachmentRendererID(uint32_t index) const 
    {
        
    }
    
    const FrameBufferSpec& OpenGLFrameBuffer::GetSpecification() const 
    {
        
    }
    
    void OpenGLFrameBuffer::Reinit()
    {
        // 如果原先已经初始化过了就先清理
        if(m_RendererID)
        {
            Delete();
        }
        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

    }
}