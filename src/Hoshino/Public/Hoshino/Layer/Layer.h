#pragma once
#include "HoshinoPch.h"
#include "Hoshino/Macro.h"
#include "Hoshino/Event/Event.h"

namespace Hoshino
{
	class HOSHINO_API Layer
	{
	public:
		Layer(const std::string& name) : m_Name(name) {}
		virtual ~Layer() = default;
		virtual void OnEvent(Event& event) = 0;
		virtual void OnUpdate() = 0;
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		std::string GetName()
		{
			return m_Name;
		}

	protected:
		std::string m_Name;
	};
} // namespace Hoshino
