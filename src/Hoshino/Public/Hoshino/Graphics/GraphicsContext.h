#pragma once

#include "HoshinoPch.h"

namespace Hoshino
{
	class HOSHINO_API GraphicsContext
	{
	public:
		GraphicsContext() {}
		virtual ~GraphicsContext() = default;

	public:
		virtual void Init() = 0;
		virtual void SwapBuffer() = 0;
	};
} // namespace Hoshino