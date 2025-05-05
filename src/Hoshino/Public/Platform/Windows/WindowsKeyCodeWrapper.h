#pragma once
#include "Hoshino/KeyCode.h"

namespace Hoshino
{
	KeyCode GlfwKeyToKeyCode(int glfwKeyCode);
	int KeyCodeToGlfwKey(KeyCode keyCode);
}
