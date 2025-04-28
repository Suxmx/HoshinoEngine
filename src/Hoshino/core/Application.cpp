#include "Application.h"

namespace Hoshino {
    Application::Application()
    {

    }
    Application::~Application()
    {

    }
    void Application::Run()
    {
        while (true)
        {
            // Main loop code here
        }
    }
    Application* CreateApplication()
    {
        return new Application();
    }
}