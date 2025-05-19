#pragma once

#include "HoshinoMin.h"
#include "Hoshino/Graphics/RenderAPI.h"
#include "Mesh.h"

namespace Hoshino
{
	class HOSHINO_API RenderCommand
	{
	public:
		inline static Ref<RenderAPI> GetRenderAPI()
		{
			return s_RenderAPI;
		}

		inline static void SetClearColor(glm::vec4 color)
		{
			s_RenderAPI->SetClearColor(color);
		}
		inline static void Clear()
		{
			s_RenderAPI->Clear();
		}
		inline static void DrawIndexed(Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}
		inline static void DrawIndexed(Ref<VertexArray>& vertexArray,Ref<MeshSource> meshSource,uint32_t submeshIndex)
		{
			s_RenderAPI->DrawIndexed(vertexArray,meshSource, submeshIndex);
		}
		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RenderAPI->SetViewport(x, y, width, height);
		}

	protected:
		static Ref<RenderAPI> s_RenderAPI;
	};
} // namespace Hoshino