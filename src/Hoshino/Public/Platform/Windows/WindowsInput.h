#pragma once
#include "Hoshino/Input.h"

namespace Hoshino
{
	class HOSHINO_API WindowsInput : public Input
	{
	public:
		WindowsInput() {}
		virtual ~WindowsInput() = default;

	public:
		virtual bool IsKeyPressedImpl(KeyCode keycode) override;
		virtual bool IsMouseButtonPressedImpl(MouseButtonCode button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
	};
} // namespace Hoshino
