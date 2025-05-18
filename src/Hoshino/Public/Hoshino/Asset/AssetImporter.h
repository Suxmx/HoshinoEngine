#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/Mesh.h"
namespace Hoshino
{
	class HOSHINO_API AssetImporter
	{
	public:
		static Ref<MeshSource> ImportMesh(const std::string& path);

	private:
		static void TraverseNodes(Ref<MeshSource> meshSource, void* assimpNode, uint32_t nodeIndex,
		                          const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);
	};
} // namespace Hoshino