#include "Platform/OpenGL/OpenGLRenderAPI.h"
#include "Hoshino/Graphics/Mesh.h"

#include <glad/glad.h>

namespace Hoshino
{
	void OpenGLRenderAPI::SetClearColor(glm::vec4 color)
	{
		glClearColor(color.x, color.y, color.z, color.w);
	}

	void OpenGLRenderAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRenderAPI::DrawIndexed(Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
	
	void OpenGLRenderAPI::DrawIndexed(Ref<VertexArray>& vertexArray, Ref<MeshSource> meshSource, uint32_t submeshIndex)
	{
		auto& submesh = meshSource->m_Submeshes[submeshIndex];
		glDrawElements(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(submesh.BaseIndex * sizeof(uint32_t)));
	}
	
	void OpenGLRenderAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

} // namespace Hoshino