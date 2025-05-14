#pragma once
#include "HoshinoMin.h"
namespace Hoshino
{

	enum class TextureFormat
	{
		NONE = 0,
		RGB = 1,
		RGBA = 2,
	};
	class Texture
	{
	public:
		Texture(const std::string& path){}
		virtual ~Texture()=default;

		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void Unbind() = 0;

		inline uint32_t GetWidth() const
		{
			return m_Width;
		}
		inline uint32_t GetHeight() const
		{
			return m_Height;
		}
        static Ref<Texture> Create(const std::string& path);

	protected:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		TextureFormat m_Format = TextureFormat::NONE;
	};
} // namespace Hoshino