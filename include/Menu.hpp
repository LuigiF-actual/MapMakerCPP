#pragma once

#include <vector>
#include <string>

#include <raylib.h>

enum class MenuAction : unsigned char
{
	NONE,
	NEW_PROJECT,
	OPEN_PROJECT,
	SAVE_PROJECT,
	SEND_FORM
};



struct NewProjectConfig
{
	char projectName[16] = "************";
	int gridWidth = 1;
	int gridHeight = 1;
};


class NewProject_UI
{
public:
	NewProject_UI( MenuAction& menuAction, NewProjectConfig& projectConfig)
		:
		m_MenuState(menuAction),
		m_Config(projectConfig)

	{};

	void draw()
	{
		int result = GuiWindowBox(m_WinBoxBody, "New Projects Config");
		if (result == 0)
		{
			GuiLabel(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y+30,m_WinBoxBody.width,120.0f }, "Project Name");
			if (GuiTextBox(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 + 30,m_WinBoxBody.width,120.0f }, m_Config.projectName, 15, m_VariableCMode)) m_VariableCMode = !m_VariableCMode;
			GuiLabel(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 2 + 30,m_WinBoxBody.width,120.0f }, "Grid Width");
			if (GuiValueBox(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 3 + 30,m_WinBoxBody.width,120.0f }, " ", &m_Config.gridWidth, 1, 1'000, m_VariableAMode)) m_VariableAMode = !m_VariableAMode;
			GuiLabel(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 4 + 30,m_WinBoxBody.width,120.0f }, "Grid Height");
			if (GuiValueBox(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 5 + 30,m_WinBoxBody.width,120.0f }, " ", &m_Config.gridHeight, 1, 1'000, m_VariableBMode)) m_VariableBMode = !m_VariableBMode;
		}
		else {
			m_MenuState = MenuAction::NONE;
		}
	}


private:

	bool m_VariableAMode = false;
	bool m_VariableBMode = false;
	bool m_VariableCMode = false;

	MenuAction& m_MenuState;

	NewProjectConfig& m_Config;

	Rectangle m_WinBoxBody = { GetScreenWidth() / 2.0f - 440.0f,50.0f, 440.0f,880.0f };
};


class OpenProject_UI
{
public:
	OpenProject_UI()
	{}

	~OpenProject_UI()
	{}

private:

};


class Menu
{
public:

	Menu() = default;

	void update()
	{

		if (IsWindowResized() || m_MenuOpened)
		{
			m_ButtonH = GetScreenHeight() * 10 / 100.0f;
			m_ButtonW = GetScreenWidth() * 15 / 100.0f;
			m_MarginLeft = (GetScreenWidth() / 2.0f) - m_ButtonW / 2.0f;
			m_MarginTop = GetScreenHeight() * 25 / 100.0f;
			GuiSetStyle(DEFAULT, TEXT_SIZE, m_ButtonH * 25 / 100);
			m_MenuOpened = false;
		}


		switch (m_BtnState)
		{
		case MenuAction::NONE:
			DrawRectangle(0.0f, 0.0f, GetScreenWidth(), GetScreenHeight(), Color{ 70,130,180,255 });
			if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop, m_ButtonW, m_ButtonH }, "#191#New Project") == 1) 
			{
				m_BtnState = MenuAction::NEW_PROJECT;
			}
			if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop + m_ButtonH, m_ButtonW, m_ButtonH }, "#191#Open Project") == 1) 
			{
				m_BtnState = MenuAction::OPEN_PROJECT;
			}
			if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop + (m_ButtonH * 2), m_ButtonW, m_ButtonH }, "#191#Save Project") == 1) 
			{
				m_BtnState = MenuAction::SAVE_PROJECT;
			}
			break;
		case MenuAction::NEW_PROJECT:
			DrawRectangle(0.0f, 0.0f, GetScreenWidth(), GetScreenHeight(), Color{ 70,130,180,255 });
			m_NewProject.draw();
			break;
		}


	}

	void setMenu(bool val)
	{
		m_MenuOpened = val;
	}

	MenuAction& getMenuState()
	{
		return m_BtnState;
	}

	void setMenuState(MenuAction state)
	{
		m_BtnState = state;
	}


private:
	bool m_MenuOpened = false;
	float m_MarginLeft = 0.0f;
	float m_MarginTop = 0.0f;
	float m_ButtonH = 0.0f;
	float m_ButtonW = 0.0f;
	MenuAction m_BtnState = MenuAction::NONE;

	NewProjectConfig m_NewProjectConfig;

	NewProject_UI m_NewProject{m_BtnState,m_NewProjectConfig};
};

