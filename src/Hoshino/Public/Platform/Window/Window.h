#include "Core/HoshinoPch.h"
#include "Platform/Window/WindowProps.h"
#include "Event/Event.h"

namespace Hoshino
{
	class HOSHINO_API Window
	{
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(WindowProps& windowProps) {}
		virtual ~Window() = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void OnUpdate() = 0;
		// Window
		virtual void SetEventCallbackFn(const EventCallbackFn& fn) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual void IsVSync() = 0;
		// Create
		static Window* Create(const WindowProps& = WindowProps());
	};
} // namespace Hoshino