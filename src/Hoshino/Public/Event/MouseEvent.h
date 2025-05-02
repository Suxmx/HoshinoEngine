#pragma once

#include "Event/Event.h"
#include <sstream>

namespace Hoshino
{
	class HOSHINO_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(int x, int y) : m_PosX(x), m_PoxY(y) {}
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseMove)

		inline int GetX() { return m_PosX; }

		inline int GetY() { return m_PoxY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent : (" << m_PosX << ", " << m_PoxY << ")";
			return ss.str();
		}

	private:
		int m_PosX,m_PoxY;
	};

	class HOSHINO_API MouseButtonEvent : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
	protected:
		MouseButtonEvent(int buttonCode) : m_ButtonCode(buttonCode) {}
		int m_ButtonCode;
	};

	class HOSHINO_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int buttonCode) : MouseButtonEvent(buttonCode) {}
		EVENT_CLASS_TYPE(MouseButtonPress)
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent : (" << m_ButtonCode << ")";
			return ss.str();
		}
	};

	class HOSHINO_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int buttonCode) : MouseButtonEvent(buttonCode) {}
		EVENT_CLASS_TYPE(MouseButtonRelease)
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent : (" << m_ButtonCode << ")";
			return ss.str();
		}
	};

} // namespace Hoshino