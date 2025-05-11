#pragma once
#include "Hoshino/Graphics/RenderAPI.h"

namespace Hoshino
{
	class HOSHINO_API OpenGLRenderAPI : public RenderAPI
	{
	public:
		void SetClearColor(glm::vec4 color) override;
		void Clear() override;
		void DrawIndexed(std::shared_ptr<VertexArray>& vertexArray) override;
	};
} // namespace Hoshino