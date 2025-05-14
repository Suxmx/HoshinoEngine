#pragma once

#include "HoshinoMin.h"
#include "Hoshino/KeyCode.h"
namespace Hoshino
{
	class HOSHINO_API Input
	{
	public:
		Input()
		{
		}
		virtual ~Input() = default;

	public:
		// Statics
		inline static bool IsKeyPressed(KeyCode keycode)
		{
			return s_Instance->IsKeyPressedImpl(keycode);
		}
		inline static bool IsMouseButtonPressed(MouseButtonCode button)
		{
			return s_Instance->IsMouseButtonPressedImpl(button);
		}
		inline static float GetMouseX()
		{
			return s_Instance->GetMouseXImpl();
		}
		inline static float GetMouseY()
		{
			return s_Instance->GetMouseYImpl();
		}
		inline static std::pair<float, float> GetMousePosition()
		{
			return s_Instance->GetMousePositionImpl();
		}

	protected:
		// Virtual Impls
		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;

	private:
		static Ref<Input> s_Instance;
	};
} // namespace Hoshino