#pragma once
#include "Core/HoshinoCore.h"
#include "Platform/Window/Window.h"

namespace Hoshino
{
	class HOSHINO_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();

	private:
		std::unique_ptr<Window> m_Window;
	};
	Application* CreateApplication();
} // namespace Hoshino