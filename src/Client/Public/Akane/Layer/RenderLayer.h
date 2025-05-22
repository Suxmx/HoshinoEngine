#pragma once

#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Graphics/FrameBuffer.h"

template <typename T>
using Ref = Hoshino::Ref<T>;

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
		Ref<Hoshino::MeshSource> m_MeshSource;
        Ref<Hoshino::Framebuffer> m_Framebuffer;
        Ref<Hoshino::Shader> m_GbufferShader;
        Ref<Hoshino::Shader> m_LightingShader;
	};
    
    
}