#pragma once

#include "HoshinoMin.h"
#include "Hoshino/Graphics/RenderAPI.h"

namespace Hoshino
{
	class HOSHINO_API RenderCommand
	{
	public:
		inline static RenderAPI* GetRenderAPI()
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
		inline static void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}

	protected:
		static RenderAPI* s_RenderAPI;
	};
} // namespace Hoshino