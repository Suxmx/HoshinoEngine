#include "HoshinoPch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hoshino
{
	// ---------------VertexShader---------------

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return new OpenGLVertexBuffer(vertices, size);
	}
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) : VertexBuffer(vertices, size)
	{
		glCreateBuffers(1, &m_BufferId);
		glNamedBufferData(m_BufferId, size, vertices, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// ---------------IndexShader---------------

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		return new OpenGLIndexBuffer(indices, size);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t size) : IndexBuffer(indices, size)
	{
		glCreateBuffers(1, &m_BufferId);
		glNamedBufferData(m_BufferId, size, indices, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLIndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

} // namespace Hoshino