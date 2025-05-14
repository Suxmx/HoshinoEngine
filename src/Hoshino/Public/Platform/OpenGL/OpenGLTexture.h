#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/Texture.h"
namespace Hoshino
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& path);
		~OpenGLTexture() override;
		void Bind(uint32_t slot = 0) override;
		void Unbind() override;
		inline uint32_t GetRendererID() const
		{
			return m_RendererID;
		}

	protected:
		uint32_t m_RendererID;
	};
    
} // namespace Hoshino