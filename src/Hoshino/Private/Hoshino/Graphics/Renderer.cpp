#include "Hoshino/Graphics/Renderer.h"
#include "Hoshino/Graphics/RenderCommand.h"

namespace Hoshino
{
	Ref<Renderer::RenderData> Renderer::s_RenderData = Ref<Renderer::RenderData>(new Renderer::RenderData());
	void Renderer::Init()
	{
		RenderCommand::Init();
	}
	
	void Renderer::BeginScene(Ref<Camera>& camera)
	{
		s_RenderData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
		s_RenderData->ViewPos = camera->GetPosition();
	}

	void Renderer::EndScene() {}

	void Renderer::Submit(Ref<VertexArray>& vertexArray, Ref<Shader>& shader,Transform transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_RenderData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform.GetTransformMatrix());
		vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
	}
	
	void Renderer::RenderStaticMesh(Ref<MeshSource> meshSource, Ref<Shader>& shader,
										 Transform transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjection", s_RenderData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform.GetTransformMatrix());
		meshSource->GetVertexArray()->Bind();
		for (int i = 0; i < meshSource->m_Submeshes.size(); i++)
		{
			auto& submesh = meshSource->m_Submeshes[i];
			RenderCommand::DrawIndexed(meshSource->m_VertexArray, meshSource, i);
		}
	}
	
	Ref<Shader> Renderer::GetDefaultShader()
	{
		if( s_RenderData->DefaultShader == nullptr)
		{
			s_RenderData->DefaultShader =
			    Shader::CreateFromFile("Res/Shader/Vert/Vertex.vert", "Res/Shader/Frag/Normal.glsl");
		}
		return s_RenderData->DefaultShader;
	}
} // namespace Hoshino