#pragma once
#include "HoshinoMin.h"

namespace Hoshino
{

	enum class ShaderDataType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:
			return 4;
		case ShaderDataType::Float2:
			return 4 * 2;
		case ShaderDataType::Float3:
			return 4 * 3;
		case ShaderDataType::Float4:
			return 4 * 4;
		case ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case ShaderDataType::Int:
			return 4;
		case ShaderDataType::Int2:
			return 4 * 2;
		case ShaderDataType::Int3:
			return 4 * 3;
		case ShaderDataType::Int4:
			return 4 * 4;
		case ShaderDataType::Bool:
			return 1;
		}

		CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType DataType;
		std::string Name;
		bool Normalized;
		uint32_t Offset;
		BufferElement(std::string name,ShaderDataType type, bool normalized = false, uint32_t offset = 0) :
		    DataType(type), Name(name), Normalized(normalized)
		{}
		uint32_t GetComponentCount() const
		{
			switch (DataType)
			{
			case ShaderDataType::Float:
				return 1;
			case ShaderDataType::Float2:
				return 2;
			case ShaderDataType::Float3:
				return 3;
			case ShaderDataType::Float4:
				return 4;
			case ShaderDataType::Mat3:
				return 3 * 3;
			case ShaderDataType::Mat4:
				return 4 * 4;
			case ShaderDataType::Int:
				return 1;
			case ShaderDataType::Int2:
				return 2;
			case ShaderDataType::Int3:
				return 3;
			case ShaderDataType::Int4:
				return 4;
			case ShaderDataType::Bool:
				return 1;
			}

			CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class HOSHINO_API BufferLayout
	{
	public:
		BufferLayout() {}
		BufferLayout(std::initializer_list<BufferElement> elements) : m_BufferElements(elements)
		{
			uint32_t offset = 0, stride = 0;
			for (auto& element : m_BufferElements)
			{
				element.Offset = offset;
				stride += ShaderDataTypeSize(element.DataType);
				offset += ShaderDataTypeSize(element.DataType);
			}
			m_Stride = stride;
		}

		inline uint32_t GetStride() const
		{
			return m_Stride;
		}

		std::vector<BufferElement>::iterator begin()
		{
			return m_BufferElements.begin();
		}
		std::vector<BufferElement>::iterator end()
		{
			return m_BufferElements.end();
		}
		std::vector<BufferElement>::const_iterator begin() const
		{
			return m_BufferElements.begin();
		}
		std::vector<BufferElement>::const_iterator end() const
		{
			return m_BufferElements.end();
		}

	private:
		std::vector<BufferElement> m_BufferElements;
		uint32_t m_Stride;
	};
} // namespace Hoshino