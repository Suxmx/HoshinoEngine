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
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		inline static Ref<SceneData> GetSceneData()
		{
			return s_SceneData;
		}

	private:
		static Ref<SceneData> s_SceneData;
	};
} // namespace Hoshino