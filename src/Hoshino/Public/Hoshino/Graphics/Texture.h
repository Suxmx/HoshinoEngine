#pragma once
#include "HoshinoMin.h"
namespace Hoshino
{

	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
		Depth = 3,
		Depth24_Stencil8 = 4,
	};
	class TextureSpec
	{
	public:
		TextureSpec(TextureFormat format, uint32_t width, uint32_t height) :
		    Format(format), Width(width), Height(height)
		{}
		TextureFormat Format;
		uint32_t Width, Height;
	};
	class Texture
	{
	public:
		Texture(const std::string& path) {}
		Texture(const TextureSpec& spec) : m_Width(spec.Width), m_Height(spec.Height), m_Format(spec.Format)
		{}
		virtual ~Texture() = default;

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

		inline uint32_t GetRendererID() const
		{
			return m_RendererID;
		}
		static Ref<Texture> Create(const std::string& path);
		static Ref<Texture> Create(const TextureSpec& spec);

	protected:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		TextureFormat m_Format = TextureFormat::None;
		uint32_t m_RendererID;
	};
} // namespace Hoshino