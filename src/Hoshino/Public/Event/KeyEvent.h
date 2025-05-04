
#pragma once

#include "Event/Event.h"
#include "Enum/KeyCode.h"
#include "Core/HoshinoPch.h"
#ifdef MAGIC_ENUM
#include <magic_enum/magic_enum.hpp>
#endif

namespace Hoshino
{
	class HOSHINO_API KeyEvent : public Event
	{
	public:
		KeyCode GetKeyCode() const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(KeyCode keycode) : m_KeyCode(keycode) {}
		KeyCode m_KeyCode;
	};

	class HOSHINO_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeatCount) :
		    KeyEvent(keycode), m_RepeatCount(repeatCount)
		{}
		int GetRepeatCount() const { return m_RepeatCount; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << " (" << ENUM_NAME(m_KeyCode) << "), Repeat Count: " << m_RepeatCount;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPress)
	protected:
		int m_RepeatCount;
	};

	class HOSHINO_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << " (" << ENUM_NAME(m_KeyCode) << ")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyRelease)
	};
} // namespace Hoshino
