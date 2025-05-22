#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Transform.h"

namespace Hoshino
{
	class Shader;
	class RenderObject
	{
	public:
		RenderObject() {
		}
		virtual ~RenderObject() = default;

	public:
		virtual void Render(Ref<Shader> shader=nullptr) = 0;

	public:
		Ref<Transform> TransformRef = CreateRef<Transform>();
	};
} // namespace Hoshino