#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Application.h"
#include "Hoshino/Entry.h"
#include "Hoshino/Layer/ImGuiLayer.h"
// Akane
#include "Akane/SandboxApplication.h"
#include "Akane/Layer/TestLayer.h"
namespace Akane
{
	Hoshino::Application* CreateApp()
	{
		auto app = new Akane::SandboxApplication();
		app->PushLayer(new TestLayer("TestLayer"));
		app->PushLayer(new Hoshino::ImGuiLayer());

		return app;
	}
} // namespace Akane

int main()
{
	Hoshino::InitEngine(Akane::CreateApp);
	return 0;
}