#pragma once
#include "Hoshino/HoshinoCore.h"
#include "Application.h"

namespace Hoshino
{
	HOSHINO_API void InitEngine(std::function<Application*()> createAppFn); // 需要在client中被调用
}