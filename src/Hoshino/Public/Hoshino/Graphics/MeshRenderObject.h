#pragma once
#include "Hoshino/Graphics/RenderObject.h"
#include "Hoshino/Graphics/Mesh.h"

namespace Hoshino
{
	class Shder;
	class HOSHINO_API MeshRenderObject : public RenderObject
	{
	public:
		MeshRenderObject(Ref<MeshSource> meshSource) : m_MeshSource(meshSource) {}

		void Render(Ref<Shader> shader = nullptr) override;

	private:
		Ref<MeshSource> m_MeshSource;
	};
} // namespace Hoshino