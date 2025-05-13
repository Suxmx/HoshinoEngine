#pragma once

#include "Hoshino/Graphics/VertexArray.h"
#include "Hoshino/Graphics/BufferLayout.h"

namespace Hoshino
{

	class HOSHINO_API OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() {}

		void Bind() override;
		void Unbind() override;
        
		void AddVertexBuffer(Ref<VertexBuffer> vb) override;
		void AddIndexBuffer(Ref<IndexBuffer> eb) override;
		

    private:
        uint32_t m_VertexArrayId;
        uint32_t m_AttriPointerIndex=0;
	};
} // namespace Hoshino