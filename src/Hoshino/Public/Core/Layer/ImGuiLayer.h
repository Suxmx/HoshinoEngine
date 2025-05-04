#pragma once
#include "Core/Layer/Layer.h"
#include "HoshinoPch.h"

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
	};
} // namespace Hoshino