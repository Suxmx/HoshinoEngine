#pragma once
#include "Hoshino/Layer/Layer.h"
#include "HoshinoPch.h"

namespace Hoshino
{
	class HOSHINO_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") {}
		~ImGuiLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		virtual void Begin();
		virtual void End();

		void BeginDockSpace();
		void EndDockSpace();

	private:
		float m_Time = 0;
		bool m_BlockEvents = true;
		bool m_DockSpaceOpen = true;
		bool m_FullscreenDockSpace = true;
	};
	
} // namespace Hoshino