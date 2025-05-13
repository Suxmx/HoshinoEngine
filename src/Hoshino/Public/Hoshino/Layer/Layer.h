#pragma once
#include "HoshinoPch.h"
#include "Hoshino/Macro.h"
#include "Hoshino/Event/Event.h"
#include "Hoshino/TimeStep.h"

namespace Hoshino
{
	class HOSHINO_API Layer
	{
	public:
		Layer(const std::string& name) : m_Name(name) {}
		virtual ~Layer() = default;
		virtual void OnEvent(Event& event){}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnImGuiRender() {}
		std::string GetName()
		{
			return m_Name;
		}

	protected:
		std::string m_Name;
	};
} // namespace Hoshino
