#pragma once
#include "Core/HoshinoCore.h"
#include "Core/Layer/Layer.h"

namespace Akane
{
    class TestLayer : public Hoshino::Layer
    {
        public:
        TestLayer(const std::string& name): Hoshino::Layer(name){}
        void OnUpdate() override;
        void OnEvent(Hoshino::Event &event) override;
    };
}