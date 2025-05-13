#include "Hoshino/Graphics/Renderer.h"
#include "Hoshino/Graphics/RenderCommand.h"

namespace Hoshino
{
	Ref<Renderer::SceneData> Renderer::s_SceneData = Ref<Renderer::SceneData>(new Renderer::SceneData());
	void Renderer::BeginScene(Ref<Camera>& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {}

	void Renderer::Submit(Ref<VertexArray>& vertexArray, Ref<Shader>& shader)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
	}
} // namespace Hoshino