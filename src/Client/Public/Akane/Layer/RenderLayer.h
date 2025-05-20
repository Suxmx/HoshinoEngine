#pragma once

#include "Hoshino/HoshinoCore.h"

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
		Ref<Hoshino::Shader> m_Shader;
		Ref<Hoshino::Shader> m_BlueShader;
        Ref<Hoshino::Texture> m_Texture;
        Ref<Hoshino::Scene> m_Scene;
	};
    
    
}