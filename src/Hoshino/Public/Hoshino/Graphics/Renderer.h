#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/Camera.h"
#include "Hoshino/Graphics/VertexArray.h"
#include "Hoshino/Graphics/Shader.h"
#include "Hoshino/Transform.h"

namespace Hoshino
{
	class MeshSource;
	class HOSHINO_API Renderer
	{
	public:
		static void BeginScene(Ref<Camera>& camera);

		static void EndScene();

		static void Submit(Ref<VertexArray>& vertexArray, Ref<Shader>& shader,
		                   Transform transform = Transform());
		static void RenderStaticMesh(Ref<MeshSource> meshSource, Ref<Shader>& shader,
		                             Transform transform = Transform());
		static Ref<Shader> GetDefaultShader();
		struct RenderData
		{
			glm::mat4 ViewProjectionMatrix;
			Ref<Shader> DefaultShader = nullptr; // 默认着色器
		};
		inline static Ref<RenderData> GetRenderData()
		{
			return s_RenderData;
		}

	private:
		static Ref<RenderData> s_RenderData;
	};
} // namespace Hoshino