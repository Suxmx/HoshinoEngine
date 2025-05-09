#pragma once

#include "Hoshino/Graphics/Buffer.h"
#include <cstdint>

namespace Hoshino
{
	class HOSHINO_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer() = default;
		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_BufferId;
	};

	class HOSHINO_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* vertices, uint32_t size);
		~OpenGLIndexBuffer() = default;
		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_BufferId;
	};
} // namespace Hoshino