#pragma once
#include "Hoshino/Graphics/FrameBuffer.h"

namespace Hoshino
{
	class OpenGLFrameBuffer : public Framebuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpec& spec);
		~OpenGLFrameBuffer();
		void Bind() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;
		int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		void ClearAttachment(uint32_t attachmentIndex, int value) override;

		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override;

		const FrameBufferSpec& GetSpecification() const override;
		Ref<Texture> GetColorAttachmentTexture(uint32_t index = 0) const override;

	private:
		void Reinit();
		void Delete();

	private:
		uint32_t m_RendererID = 0;
		std::vector<uint32_t> m_ColorAttachments;
		std::vector<Ref<Texture>> m_ColorAttachmentTextures;
		Ref<Texture> m_DepthAttachmentTexture;
		uint32_t m_DepthAttachment = 0;
		int m_AttachTextureIndex = -1;
		FrameBufferTextureSpec m_DepthAttachmentSpec;
		std::vector<FrameBufferTextureSpec> m_ColorAttachmentSpecifications;
	};
} // namespace Hoshino