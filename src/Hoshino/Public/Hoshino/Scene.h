#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Graphics/Camera.h"
#include "Hoshino/Graphics/RenderObject.h"
namespace Hoshino
{
	// now for render only
	class HOSHINO_API Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		void Render();
        void PushRenderObject(const Ref<RenderObject>& renderObject)
        {
            m_RenderObjects.push_back(renderObject);
        }
		std::vector<Ref<RenderObject>>& GetRenderObjects()
		{
			return m_RenderObjects;
		}

		private : std::vector<Ref<RenderObject>> m_RenderObjects;
	};
} // namespace Hoshino