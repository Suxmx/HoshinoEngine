#pragma once

#include "HoshinoMin.h"
#include "Hoshino/Graphics/Buffer.h"

namespace Hoshino
{
	class HOSHINO_API VertexArray
	{
	public:
		VertexArray() {}
		~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void AddVertexBuffer(Ref<VertexBuffer> vb) = 0;
		virtual void AddIndexBuffer(Ref<IndexBuffer> eb) = 0;

		inline Ref<VertexBuffer> GetVertexBuffer()
		{
			return m_VertexBuffer;
		}
		inline Ref<IndexBuffer> GetIndexBuffer()
		{
			return m_IndexBuffer;
		}
		static Ref<VertexArray> Create();

	protected:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};
} // namespace Hoshino