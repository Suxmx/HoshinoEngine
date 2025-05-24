#pragma once

#include "Hoshino/HoshinoCore.h"
#include "Hoshino/Graphics/FrameBuffer.h"
#include "HoshinoMin.h"


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
		Hoshino::Ref<Hoshino::MeshSource> m_MeshSource;
		Hoshino::Ref<Hoshino::Framebuffer> m_Framebuffer;
		Hoshino::Ref<Hoshino::Shader> m_GbufferShader;
		Hoshino::Ref<Hoshino::Shader> m_LightingShader;
		std::vector<glm::vec3> m_LightPositions;
		std::vector<glm::vec3> m_LightColors;
	};
    
    
}