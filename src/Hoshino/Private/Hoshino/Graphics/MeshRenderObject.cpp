#include "Hoshino/Graphics/MeshRenderObject.h"
#include "Hoshino/Graphics/Renderer.h"
#include "Hoshino/Graphics/RenderCommand.h"
#include "Hoshino/Graphics/Material.h"
#include "Hoshino/Graphics/Shader.h"
namespace Hoshino
{
	void MeshRenderObject::Render(Ref<Shader> shader)
	{
		if (m_MeshSource)
		{
			m_MeshSource->GetVertexArray()->Bind();
			for (int i = 0; i < m_MeshSource->m_Submeshes.size(); i++)
			{
				auto& submesh = m_MeshSource->m_Submeshes[i];
				Ref<Material>& material = m_MeshSource->m_Materials[submesh.MaterialIndex];
				Ref<Shader>& meshShader = material->GetShader();
				
				if(shader != nullptr)
				{
					material->SetShader(shader);
				}
				material->Apply();
				material->GetShader()->UploadUniformMat4("u_ViewProjection",
				                                         Renderer::GetRenderData()->ViewProjectionMatrix);
				material->GetShader()->UploadUniformMat4("u_Transform", TransformRef->GetTransformMatrix());

				auto vao = m_MeshSource->GetVertexArray();
				vao->Bind();
				RenderCommand::DrawIndexed(vao, m_MeshSource, i);
				material->SetShader(meshShader);
			} 
		}
	}
} // namespace Hoshino