#include "Hoshino/Entry.h"
#include "Hoshino/Graphics/RenderCommand.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
namespace Hoshino {
	static std::function<Application*()> s_CreateAppFn;
	static bool s_Inited = false;

	void InitEngine(std::function<Application*()> createAppFn)
	{
		if (!s_Inited)
		{
			s_Inited = true;
			s_CreateAppFn = createAppFn;
			Hoshino::Log::Init();
			CORE_INFO("Hoshino Engine is starting...");
			CORE_ASSERT(s_CreateAppFn, "需要先设置CreateAppFn!")
			Application* app = s_CreateAppFn();
			RenderCommand::Init();
			app->Run();
			delete app;
		}
	}
}
