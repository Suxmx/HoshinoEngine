#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Application.h"
#include "Hoshino/Entry.h"
// Akane
#include "Akane/SandboxApplication.h"
#include "Akane/Layer/RenderLayer.h"
#include "Akane/Layer/ClientImguiLayer.h"
namespace Akane
{
	Hoshino::Application* CreateApp()
	{
		auto app = new Akane::SandboxApplication();
		app->PushLayer(new RenderLayer());
		app->PushLayer(new ClientImguiLayer());
		return app;
	}
} // namespace Akane

int main()
{
	Hoshino::InitEngine(Akane::CreateApp);
	return 0;
}