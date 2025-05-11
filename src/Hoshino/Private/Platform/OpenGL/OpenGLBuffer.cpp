#include "HoshinoPch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hoshino
{
#pragma region VertexBuffer

	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		return std::make_shared<OpenGLVertexBuffer>(vertices, size);
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
#pragma endregion VertexBuffer

#pragma region IndexBuffer

	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		return std::make_shared<OpenGLIndexBuffer>(indices, count);
	}
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : IndexBuffer(indices, count)
	{
		glCreateBuffers(1, &m_BufferId);
		glNamedBufferData(m_BufferId, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
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