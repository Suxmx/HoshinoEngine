#pragma once
#include "Platform/Window/Window.h"

namespace Hoshino
{
	class HOSHINO_API WindowsWindow : public Window
	{
	public:
		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }

	private:
		struct WindowData
		{
			unsigned int Height;
			unsigned int Width;
			std::string Title;
		};
		WindowData m_Data;
	};
} // namespace Hoshino