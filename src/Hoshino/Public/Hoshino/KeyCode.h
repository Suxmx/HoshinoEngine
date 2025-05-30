#pragma once
#include <imgui.h>
namespace Hoshino
{
	enum class KeyCode
	{
		NONE, // 0
		// Printable keys
		Space,        // 1
		Apostrophe,   // 2
		Comma,        // 3
		Minus,        // 4
		Period,       // 5
		Slash,        // 6
		Key0,         // 7
		Key1,         // 8
		Key2,         // 9
		Key3,         // 10
		Key4,         // 11
		Key5,         // 12
		Key6,         // 13
		Key7,         // 14
		Key8,         // 15
		Key9,         // 16
		Semicolon,    // 17
		Equal,        // 18
		A,            // 19
		B,            // 20
		C,            // 21
		D,            // 22
		E,            // 23
		F,            // 24
		G,            // 25
		H,            // 26
		I,            // 27
		J,            // 28
		K,            // 29
		L,            // 30
		M,            // 31
		N,            // 32
		O,            // 33
		P,            // 34
		Q,            // 35
		R,            // 36
		S,            // 37
		T,            // 38
		U,            // 39
		V,            // 40
		W,            // 41
		X,            // 42
		Y,            // 43
		Z,            // 44
		LeftBracket,  // 45
		Backslash,    // 46
		RightBracket, // 47
		GraveAccent,  // 48
		World1,       // 49
		World2,       // 50

		// Function keys
		Escape,      // 51
		Enter,       // 52
		Tab,         // 53
		Backspace,   // 54
		Insert,      // 55
		Delete,      // 56
		Right,       // 57
		Left,        // 58
		Down,        // 59
		Up,          // 60
		PageUp,      // 61
		PageDown,    // 62
		Home,        // 63
		End,         // 64
		CapsLock,    // 65
		ScrollLock,  // 66
		NumLock,     // 67
		PrintScreen, // 68
		Pause,       // 69
		F1,          // 70
		F2,          // 71
		F3,          // 72
		F4,          // 73
		F5,          // 74
		F6,          // 75
		F7,          // 76
		F8,          // 77
		F9,          // 78
		F10,         // 79
		F11,         // 80
		F12,         // 81
		F13,         // 82
		F14,         // 83
		F15,         // 84
		F16,         // 85
		F17,         // 86
		F18,         // 87
		F19,         // 88
		F20,         // 89
		F21,         // 90
		F22,         // 91
		F23,         // 92
		F24,         // 93
		F25,         // 94

		// Keypad keys
		KP0,        // 95
		KP1,        // 96
		KP2,        // 97
		KP3,        // 98
		KP4,        // 99
		KP5,        // 100
		KP6,        // 101
		KP7,        // 102
		KP8,        // 103
		KP9,        // 104
		KPDecimal,  // 105
		KPDivide,   // 106
		KPMultiply, // 107
		KPSubtract, // 108
		KPAdd,      // 109
		KPEnter,    // 110
		KPEqual,    // 111

		// Modifier keys
		LeftShift,    // 112
		LeftControl,  // 113
		LeftAlt,      // 114
		LeftSuper,    // 115
		RightShift,   // 116
		RightControl, // 117
		RightAlt,     // 118
		RightSuper,   // 119
		Menu          // 120
	};

	enum class MouseButtonCode
	{
		LeftButton = 0,
		RightButton = 1,
		MiddleButton = 2,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
	};

	ImGuiKey HoshinoKeyToImGuiKey(KeyCode keycode);

} // namespace Hoshino