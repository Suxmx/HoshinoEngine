#include "Hoshino/Scene.h"

namespace Hoshino
{
	void Scene::Render()
	{
		for (const auto& renderObject : m_RenderObjects)
		{
			renderObject->Render();
		}
	}

} // namespace Hoshino