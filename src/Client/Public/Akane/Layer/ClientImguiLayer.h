#pragma once
#include "Hoshino/HoshinoCore.h"

namespace Akane
{
	class ClientImguiLayer : public Hoshino::Layer
	{
	public:
		ClientImguiLayer() : Hoshino::Layer("ClientImguiLayer") {}
		~ClientImguiLayer() override = default;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(Hoshino::Timestep ts) override;
		void OnImGuiRender() override;
		void OnEvent(Hoshino::Event& event) override;

	private:
		float m_Time;
	};
} // namespace Akane