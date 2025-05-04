#pragma once
#include "Core/Log.h"
#include "HoshinoPch.h"

#define EVENT_CLASS_TYPE(type)                                   \
	static EventType GetStaticType() { return EventType::type; } \
	virtual EventType GetEventType() const override              \
	{                                                            \
		return GetStaticType();                                  \
	}                                                            \
	virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) \
	virtual int GetCategoryFlags() const override { return (category); }

namespace Hoshino
{
	enum class EventType
	{
		None = 0,
		// Key
		KeyPress,
		KeyRelease,
		// Mouse
		MouseButtonPress,
		MouseButtonRelease,
		MouseMove,
		MouseScroll,
		// Window
		WindowResized,
		WindowClose,
		WindowFocus,
		WindowLostFocus,
		WindowMove,
		// App
		AppTick,
		AppRender,
		AppUpdate
	};

	enum EventCategory
	{
		EventCategoryNone = 0,
		EventCategoryInput = BIT(0),
		EventCategoryKeyboard = BIT(1),
		EventCategoryMouse = BIT(2),
		EventCategoryWindow = BIT(3),
		EventCategoryApplication = BIT(4)
	};

	class HOSHINO_API Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		inline bool IsInCategory(EventCategory category)
		{
			return (GetCategoryFlags() & static_cast<int>(category)) != 0;
		}
		bool Handled = false;

	protected:
		
	};
	inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

	class HOSHINO_API EventDispatcher
	{
		template <typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event) : m_Event(event) {}

		template <typename T>
		bool Dispatch(EventFn<T> func)
		{
			if(m_Event.GetEventType() != T::GetStaticType())return false;
			T* derivedEvent = dynamic_cast<T*>(&m_Event);
			if (derivedEvent)
			{
				m_Event.Handled = func(*derivedEvent);
				return true;
			}
			else
			{
				CORE_ERROR("EventDispatcher: Dynamic cast failed!");
				return false;
			}
		}
		
	private:
		Event& m_Event;
	};

} // namespace Hoshino
