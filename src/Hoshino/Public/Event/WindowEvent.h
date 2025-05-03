#pragma once

#include "Event/Event.h"
#include "Core/HoshinoPch.h"

namespace Hoshino
{
	class HOSHINO_API WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}
		unsigned int GetWidth() { return m_Width; }
		unsigned int GetHeight() { return m_Height; }

		EVENT_CLASS_CATEGORY(EventCategoryWindow) EVENT_CLASS_TYPE(WindowResized) std::string
		    ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizedEvent: (" << m_Width << ", " << m_Height << ")";
			return ss.str();
		}

	private:
		unsigned int m_Width, m_Height;
	};

	class HOSHINO_API WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(int x, int y) : m_PosX(x), m_PosY(y) {}
		unsigned int GetX() { return m_PosX; }
		unsigned int GetY() { return m_PosY; }

		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowMove)
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMoveEvent: (" << m_PosX << ", " << m_PosY << ")";
			return ss.str();
		}

	private:
		int m_PosX, m_PosY;
	};

	class HOSHINO_API WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowFocus)
	};

	class HOSHINO_API WindowLostFocusEvent : public Event
	{
	public:
		WindowLostFocusEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowLostFocus)
	};

	class HOSHINO_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
		EVENT_CLASS_TYPE(WindowClose)
	};
} // namespace Hoshino