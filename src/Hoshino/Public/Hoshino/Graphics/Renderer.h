#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/VertexArray.h"

namespace Hoshino
{
	class HOSHINO_API Renderer
	{
	public:
		static void BeginScene();

		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray>& vertexArray);
	};
} // namespace Hoshino