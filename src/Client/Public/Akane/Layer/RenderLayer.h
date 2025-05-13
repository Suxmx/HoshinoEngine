#pragma once

#include "Hoshino/HoshinoCore.h"


namespace Akane {
    class RenderLayer : public Hoshino::Layer {
    public:
        RenderLayer() : Layer("RenderLayer") {}
        ~RenderLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(Hoshino::Timestep ts) override;
        void OnImGuiRender() override;
        void OnEvent(Hoshino::Event& event) override;

	private:
		Hoshino::Ref<Hoshino::VertexArray> m_TriangleVa;
		Hoshino::Ref<Hoshino::VertexArray> m_SquareVa;
		Hoshino::Ref<Hoshino::Shader> m_Shader;
		Hoshino::Ref<Hoshino::Shader> m_BlueShader;
	};
    
    
}