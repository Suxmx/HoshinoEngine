#pragma once

#include "HoshinoMin.h"
#include "Hoshino/Graphics/RenderAPI.h"

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

	protected:
		static Ref<RenderAPI> s_RenderAPI;
	};
} // namespace Hoshino