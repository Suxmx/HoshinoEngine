#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/VertexArray.h"

#include <glm/glm.hpp>


namespace Hoshino
{
	class MeshSource;
	class HOSHINO_API RenderAPI
	{
	public:
		enum class API
		{
			OpenGL = 1
		};
		virtual void SetClearColor(glm::vec4 color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(Ref<VertexArray>& vertexArray) = 0;
		virtual void DrawIndexed(Ref<VertexArray>& vertexArray, Ref<MeshSource> meshSource, uint32_t submeshIndex) = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void RenderStaticMesh(Ref<MeshSource> meshSource) = 0;
		inline static API GetAPI()
		{
			return s_API;
		}

	private:
		static API s_API;
	};
} // namespace Hoshino