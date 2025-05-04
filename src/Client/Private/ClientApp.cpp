#include "Core/HoshinoCore.h"
#include "Core/Application.h"
#include "Core/Entry.h"
// Akane
#include "Akane/SandboxApplication.h"
#include "Akane/Layer/TestLayer.h"
namespace Akane
{
	Hoshino::Application* CreateApp()
	{
		auto app = new Akane::SandboxApplication();
		app->PushLayer(new TestLayer("TestLayer"));

		return app;
	}
} // namespace Akane

int main()
{
	Hoshino::InitEngine(Akane::CreateApp);
	return 0;
}