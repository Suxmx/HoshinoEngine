#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/Texture.h"
namespace Hoshino
{
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string& path);
		OpenGLTexture(const TextureSpec& spec);
		~OpenGLTexture() override;
		void Bind(uint32_t slot = 0) override;
		void Unbind() override;
	};

} // namespace Hoshino