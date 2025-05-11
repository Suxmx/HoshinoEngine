#include "Hoshino/Graphics/Renderer.h"
#include "Hoshino/Graphics/RenderCommand.h"

namespace Hoshino
{
	void Renderer::BeginScene() {}

	void Renderer::EndScene() {}

	void Renderer::Submit(std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
	}
} // namespace Hoshino