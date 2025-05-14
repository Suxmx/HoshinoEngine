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

	void Renderer::Submit(Ref<VertexArray>& vertexArray, Ref<Shader>& shader,Transform transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform.GetTransformMatrix());
		vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
	}
} // namespace Hoshino