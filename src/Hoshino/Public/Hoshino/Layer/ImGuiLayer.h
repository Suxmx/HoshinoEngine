#pragma once
#include "Hoshino/Layer/Layer.h"
#include "HoshinoPch.h"
#include "Hoshino/Application.h"

namespace Hoshino
{
	class HOSHINO_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") {}
		~ImGuiLayer() = default;
		void OnEvent(Event& event) override;
		void OnUpdate() override;
		void OnAttach() override;
		void OnDetach() override;

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
		bool OnWindowResizeEvent(WindowResizedEvent& e);

	private:
		float m_Time = 0;
	};
} // namespace Hoshino