
#pragma once

#include "Hoshino/Event/Event.h"
#include "Hoshino/KeyCode.h"
#include "HoshinoPch.h"
#ifdef MAGIC_ENUM
#include <magic_enum/magic_enum.hpp>
#endif

namespace Hoshino
{
	class HOSHINO_API KeyEvent : public Event
	{
	public:
		inline KeyCode GetKeyCode() const
		{
			return m_KeyCode;
		}
		inline int GetMods() const
		{
			return m_Mods;
		}
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(KeyCode keycode, int mods) : m_KeyCode(keycode), m_Mods(mods) {}
		KeyCode m_KeyCode;
		int m_Mods;
	};

	class HOSHINO_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int mods, int repeatCount) :
		    KeyEvent(keycode, mods), m_RepeatCount(repeatCount)
		{}
		int GetRepeatCount() const
		{
			return m_RepeatCount;
		}
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
		KeyReleasedEvent(KeyCode keycode, int mods) : KeyEvent(keycode, mods) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << " (" << ENUM_NAME(m_KeyCode) << ")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyRelease)
	};

	class HOSHINO_API KeyTypedEvent : public Event
	{
	public:
		KeyTypedEvent(unsigned int character) : m_Character(character) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << " (" << m_Character << ")";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyTyped)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

		unsigned int GetCharacter() const
		{
			return m_Character;
		}

	private:
		unsigned int m_Character;
	};
} // namespace Hoshino
