#pragma once
#include "Hoshino/Macro.h"
#include "Hoshino/Graphics/BufferLayout.h"
namespace Hoshino
{
	class HOSHINO_API VertexBuffer
	{
	public:
		VertexBuffer(float* vertices, uint32_t size) {}
		virtual ~VertexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		inline BufferLayout GetLayout()
		{
			return m_Layout;
		}
		inline void SetLayout(BufferLayout layout)
		{
			m_Layout = layout;
		}

		static Ref<VertexBuffer> Create(float* vertices, uint32_t size);

	protected:
		BufferLayout m_Layout;
	};

	class HOSHINO_API IndexBuffer
	{
	public:
		IndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count) {}
		virtual ~IndexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual uint32_t GetCount() = 0;
		static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);

	protected:
		uint32_t m_Count;	
	};
} // namespace Hoshino