#pragma once
#include "Core/Macro.h"
#include "HoshinoPch.h"
namespace Hoshino
{
	struct WindowProps
	{
		unsigned int Height;
		unsigned int Width;
		std::string Title;
		WindowProps(const std::string& title = "Hoshino Engine", unsigned int w = 1280,
		            unsigned int h = 720) : Title(title), Width(w), Height(h)
		{}
	};
} // namespace Hoshino
