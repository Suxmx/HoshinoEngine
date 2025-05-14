#include "Hoshino/Entry.h"

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
			int x, y, channels_in_file;
			stbi_load("Shader/test.png", &x, &y, &channels_in_file, 0);
			CORE_INFO("Image loaded: {0}x{1}, {2} channels", x, y, channels_in_file);
			app->Run();
			delete app;
		}
	}
}
