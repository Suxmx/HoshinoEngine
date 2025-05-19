#include "HoshinoPch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hoshino
{
#pragma region VertexBuffer

	Ref<VertexBuffer> VertexBuffer::Create(void* data, uint32_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(data, size);
	}
	OpenGLVertexBuffer::OpenGLVertexBuffer(void* data, uint32_t size) : VertexBuffer(data, size)
	{
		glCreateBuffers(1, &m_BufferId);
		glNamedBufferData(m_BufferId, size, data, GL_STATIC_DRAW);
	}

	void OpenGLVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLVertexBuffer::Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
#pragma endregion VertexBuffer

#pragma region IndexBuffer

	Ref<IndexBuffer> IndexBuffer::Create(void* data, uint32_t size)
	{
		return std::make_shared<OpenGLIndexBuffer>(data, size);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(void* data, uint32_t size) : IndexBuffer(data, size)
	{
		glCreateBuffers(1, &m_BufferId);
		glNamedBufferData(m_BufferId, size, data, GL_STATIC_DRAW);
	}

	void OpenGLIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLIndexBuffer::Unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t OpenGLIndexBuffer::GetCount()
	{
		return m_Count;
	}
#pragma endregion IndexBuffer
} // namespace Hoshino