#pragma once

#include "Core/Event/Event.h"
#include "HoshinoPch.h"

namespace Hoshino
{
	class HOSHINO_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y) : m_PosX(x), m_PoxY(y) {}
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseMove)

		inline float GetX()
		{
			return m_PosX;
		}

		inline float GetY()
		{
			return m_PoxY;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent : (" << m_PosX << ", " << m_PoxY << ")";
			return ss.str();
		}

	private:
		float m_PosX, m_PoxY;
	};

	class HOSHINO_API MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float x, float y) : m_OffsetX(x), m_OffsetY(y) {}
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseScroll)

		inline float GetOffsetX()
		{
			return m_OffsetX;
		}

		inline float GetOffsetY()
		{
			return m_OffsetY;
		}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent : (" << m_OffsetX << ", " << m_OffsetY << ")";
			return ss.str();
		}

	private:
		float m_OffsetX, m_OffsetY;
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