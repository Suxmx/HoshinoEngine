#include "Platform/Windows/WindowsInput.h"
#include "Platform/Windows/WindowsKeyCodeWrapper.h"
#include "Hoshino/Application.h"
#include <GLFW/glfw3.h>


namespace Hoshino
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, KeyCodeToGlfwKey(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(MouseButtonCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, MouseButtonCodeToGlfwMouseButton(button));
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Instance().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return {(float)xpos, (float)ypos};
	}

} // namespace Hoshino