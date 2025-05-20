#pragma once
#include "Hoshino/Graphics/RenderObject.h"
#include "Hoshino/Graphics/Mesh.h"

namespace Hoshino
{
	class HOSHINO_API MeshRenderObject : public RenderObject
	{
	public:
		MeshRenderObject(Ref<MeshSource> meshSource) : m_MeshSource(meshSource) {}

		void Render() override;

	private:
		Ref<MeshSource> m_MeshSource;
	};
} // namespace Hoshino