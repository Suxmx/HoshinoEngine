#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/Camera.h"
#include "Hoshino/Graphics/VertexArray.h"
#include "Hoshino/Graphics/Shader.h"

namespace Hoshino
{
	class HOSHINO_API Renderer
	{
	public:
		static void BeginScene(Ref<Camera>& camera);

		static void EndScene();

		static void Submit(Ref<VertexArray>& vertexArray, Ref<Shader>& shader);
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