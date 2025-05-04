#pragma once

#include "Core/Event/Event.h"
#include "HoshinoPch.h"
namespace Hoshino
{
    class HOSHINO_API AppTickEvent : public Event
    {
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
        EVENT_CLASS_TYPE(AppTick)
    };

	class HOSHINO_API AppUpdateEvent : public Event
	{
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};

	class HOSHINO_API AppRenderEvent : public Event
	{
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};
}