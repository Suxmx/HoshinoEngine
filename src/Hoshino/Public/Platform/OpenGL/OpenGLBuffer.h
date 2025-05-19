#pragma once

#include "Hoshino/Graphics/Buffer.h"
#include <cstdint>

namespace Hoshino
{
	class HOSHINO_API OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(void* data, uint32_t size);
		~OpenGLVertexBuffer() = default;
		void Bind() override;
		void Unbind() override;

	private:
		uint32_t m_BufferId;
	};

	class HOSHINO_API OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(void* data, uint32_t size);
		~OpenGLIndexBuffer() = default;
		void Bind() override;
		void Unbind() override;
		uint32_t GetCount() override;

	private:
		uint32_t m_BufferId;
	};
} // namespace Hoshino