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
		static void BeginScene(std::shared_ptr<Camera>& camera);

		static void EndScene();

		static void Submit(std::shared_ptr<VertexArray>& vertexArray, std::shared_ptr<Shader>& shader);
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		inline static SceneData* GetSceneData()
		{
			return s_SceneData;
		}

	private:
		static SceneData* s_SceneData;
	};
} // namespace Hoshino