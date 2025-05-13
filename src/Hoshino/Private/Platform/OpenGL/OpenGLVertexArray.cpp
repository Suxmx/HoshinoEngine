#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hoshino
{
	Ref<VertexArray> VertexArray::Create()
	{
		return std::make_shared<OpenGLVertexArray>();
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_VertexArrayId);
	}

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
			return GL_FLOAT;
		case ShaderDataType::Float2:
			return GL_FLOAT;
		case ShaderDataType::Float3:
			return GL_FLOAT;
		case ShaderDataType::Float4:
			return GL_FLOAT;
		case ShaderDataType::Mat3:
			return GL_FLOAT;
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
			return GL_INT;
		case ShaderDataType::Int2:
			return GL_INT;
		case ShaderDataType::Int3:
			return GL_INT;
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		}

		CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
	void OpenGLVertexArray::Bind()
	{
		glBindVertexArray(m_VertexArrayId);
	}

	void OpenGLVertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddIndexBuffer(Ref<IndexBuffer> eb)
	{
		m_IndexBuffer = eb;
		glBindVertexArray(m_VertexArrayId);
	}

	void OpenGLVertexArray::AddVertexBuffer(Ref<VertexBuffer> vb)
	{
		m_VertexBuffer = vb;
		glBindVertexArray(m_VertexArrayId);
		const auto& layout = (vb->GetLayout());
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(m_AttriPointerIndex);
			glVertexAttribPointer(m_AttriPointerIndex, element.GetComponentCount(),
			                      ShaderDataTypeToOpenGLBaseType(element.DataType),
			                      element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(),
			                      (const void*)element.Offset);
		}
	}

} // namespace Hoshino