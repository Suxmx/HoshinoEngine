#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/Texture.h"

namespace Hoshino
{
	struct FrameBufferTextureSpec
	{
		FrameBufferTextureSpec(TextureFormat format = TextureFormat::None,
		                       bool useRenderBuffer = false) :
		    Format(format), UseRenderBuffer(useRenderBuffer)
		{}
		TextureFormat Format;
		bool UseRenderBuffer;
	};

	struct FrameBufferAttachmentSpec
	{
		FrameBufferAttachmentSpec(std::initializer_list<FrameBufferTextureSpec> specs) : Attachments(specs) {}
		std::vector<FrameBufferTextureSpec> Attachments;
	};

	struct FrameBufferSpec
	{
		uint32_t Width, Height;
		FrameBufferAttachmentSpec AttachmentsSpc;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FrameBufferSpec& spec) : m_Spec(spec) {}
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FrameBufferSpec& GetSpecification() const = 0;

		virtual Ref<Texture> GetColorAttachmentTexture(uint32_t index = 0) const = 0;

		static Ref<Framebuffer> Create(const FrameBufferSpec& spec);

	protected:
		FrameBufferSpec m_Spec;
		uint32_t m_Height, m_Width;
	};
} // namespace Hoshino