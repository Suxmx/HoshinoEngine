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

		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer> vb) = 0;
		virtual void AddIndexBuffer(std::shared_ptr<IndexBuffer> eb) = 0;

		inline std::shared_ptr<VertexBuffer> GetVertexBuffer()
		{
			return m_VertexBuffer;
		}
		inline std::shared_ptr<IndexBuffer> GetIndexBuffer()
		{
			return m_IndexBuffer;
		}
		static std::shared_ptr<VertexArray> Create();

	protected:
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
} // namespace Hoshino