#pragma once
#include "HoshinoCore.h"
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
}