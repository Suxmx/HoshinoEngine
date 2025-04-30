#include "HoshinoCore.h"
using namespace std;

int main(int argc, char** argv)
{
    Hoshino::Log::Init();
    CORE_TRACE("Hoshino Engine is starting...");
    Hoshino::Application* app = Hoshino::CreateApplication();
    app->Run();
    delete app;
    return 0;
}