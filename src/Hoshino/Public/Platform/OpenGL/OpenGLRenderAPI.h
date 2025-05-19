#pragma once
#include "Hoshino/Graphics/RenderAPI.h"
#include "Hoshino/Graphics/Mesh.h"

namespace Hoshino
{
	class HOSHINO_API OpenGLRenderAPI : public RenderAPI
	{
	public:
		void SetClearColor(glm::vec4 color) override;
		void Clear() override;
		void DrawIndexed(Ref<VertexArray>& vertexArray) override;
		void DrawIndexed(Ref<VertexArray>& vertexArray, Ref<MeshSource> meshSource, uint32_t submeshIndex) override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
} // namespace Hoshino