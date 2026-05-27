#pragma once

#include <raylib.h>

class Menu
{
public:
	Menu()
	{}

	void draw()
	{
		DrawRectangle(0.0f, 0.0f, GetScreenWidth(), GetScreenHeight(), Color{ 206, 81, 240 , 255 });

		if (IsWindowResized() || m_MenuOpened)
		{
			m_ButtonH = GetScreenHeight() * 10 / 100.0f;
			m_ButtonW = GetScreenWidth() * 15 / 100.0f;
			m_MarginLeft = (GetScreenWidth() / 2.0f)- m_ButtonW /2.0f;
			m_MarginTop = GetScreenHeight() * 25 / 100.0f;
			GuiSetStyle(DEFAULT, TEXT_SIZE, m_ButtonH*25/100);
			m_MenuOpened = false;
		}

		if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop, m_ButtonW, m_ButtonH }, "#191#New Project")) m_ShowMessageBox = true;
		if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop + m_ButtonH, m_ButtonW, m_ButtonH }, "#191#Open Project")) m_ShowMessageBox = true;
		if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop + (m_ButtonH * 2), m_ButtonW, m_ButtonH }, "#191#Save Project")) m_ShowMessageBox = true;

	}

	void setMenu(bool val)
	{
		m_MenuOpened = val;
	}

private:
	bool m_ShowMessageBox = false;
	bool m_MenuOpened = false;
	float m_MarginLeft = 0.0f;
	float m_MarginTop = 0.0f;
	float m_ButtonH = 0.0f;
	float m_ButtonW = 0.0f;
};

