#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/VertexArray.h"

#include <glm/glm.hpp>

namespace Hoshino
{
	class HOSHINO_API RenderAPI
	{
	public:
		enum class API
		{
			OpenGL = 1
		};
		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray) = 0;
		inline static API GetAPI()
		{
			return s_API;
		}

	private:
		static API s_API;
	};
} // namespace Hoshino