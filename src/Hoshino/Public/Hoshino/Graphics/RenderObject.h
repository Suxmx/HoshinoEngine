#pragma once
#include "HoshinoMin.h"
#include "Hoshino/Transform.h"

namespace Hoshino
{
	class RenderObject
	{
	public:
		RenderObject() {
		}
		virtual ~RenderObject() = default;

	public:
		virtual void Render() = 0;

	public:
		Ref<Transform> TransformRef = CreateRef<Transform>();
	};
} // namespace Hoshino