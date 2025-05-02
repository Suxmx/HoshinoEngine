#pragma once
#include "Core/HoshinoCore.h"

namespace Hoshino
{
	class HOSHINO_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};
	Application* CreateApplication();
} // namespace Hoshino