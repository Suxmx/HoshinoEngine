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
		static std::shared_ptr<VertexArray> Create();
	};
} // namespace Hoshino