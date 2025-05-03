#include "Core/Application.h"
#include <memory>

namespace Hoshino {
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
    }
    Application::~Application()
    {

    }
    void Application::Run()
    {
        while (true)
        {
            m_Window->OnUpdate();
        }
    }
    Application* CreateApplication()
    {
        return new Application();
    }
}