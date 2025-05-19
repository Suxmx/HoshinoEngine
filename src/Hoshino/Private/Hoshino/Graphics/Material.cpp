#include "Hoshino/Graphics/Material.h"
#include "Hoshino/Graphics/RenderAPI.h"

#include "Platform/OpenGL/OpenGLMaterial.h"
namespace Hoshino
{
	Ref<Material> Material::Create(const Ref<Shader>& shader)
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::OpenGL:
			return CreateRef<OpenGLMaterial>(shader);
		}
		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Material> Material::Create()
	{
		switch (RenderAPI::GetAPI())
		{
		case RenderAPI::API::OpenGL:
			return CreateRef<OpenGLMaterial>();
		}
		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
} // namespace Hoshino
