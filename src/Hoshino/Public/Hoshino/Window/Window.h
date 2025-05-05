#include "HoshinoPch.h"
#include "Hoshino/Window/WindowProps.h"
#include "Hoshino/Event/Event.h"

namespace Hoshino
{
	class HOSHINO_API Window
	{
		using EventCallbackFn = std::function<void(Event&)>;

	public:
		Window(const WindowProps& windowProps) {}
		virtual ~Window() = default;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
		virtual void OnUpdate() = 0;
		// Window
		virtual void SetEventCallbackFn(const EventCallbackFn& fn) = 0;
		virtual void SetVSync(bool enable) = 0;
		virtual bool IsVSync() = 0;
		virtual void* GetNativeWindow() const = 0;
		// Create
		static Window* Create(const WindowProps& props = WindowProps());
	};
} // namespace Hoshino