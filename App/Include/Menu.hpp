#pragma once

#include <print>
#include <vector>
#include <string>
#include <regex>
#include <ranges>

#include <raylib.h>
#include <raygui.h>


#include "AppLayer.h"
#include "Config.hpp"

enum class MenuAction : unsigned char
{
	NONE,
	NEW_PROJECT,
	OPEN_PROJECT,
	SAVE_PROJECT
};


struct NewProjectConfig
{
	std::string projectName = "**************";
	int gridWidth = 1;
	int gridHeight = 1;
};


class NewProject_UI
{
public:
	NewProject_UI(MenuAction& menuAction, NewProjectConfig& projectConfig, bool& submitBool)
		:
		m_MenuState(menuAction),
		m_Config(projectConfig),
		m_SubmitNewProject(submitBool)
	{};

	~NewProject_UI() = default;

	void draw()
	{
		int result = GuiWindowBox(m_WinBoxBody, "New Projects Config");
		if (result == 0)
		{
			GuiLabel(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 30,m_WinBoxBody.width,120.0f }, "Project Name");
			if (GuiTextBox(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 + 30,m_WinBoxBody.width,120.0f }, m_Config.projectName.data(), 15, m_VariableCMode) == 1) //Controls the name
			{
				m_VariableCMode = !m_VariableCMode;
			}

			GuiLabel(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 2 + 30,m_WinBoxBody.width,120.0f }, "Grid Width");
			if (GuiValueBox(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 3 + 30,m_WinBoxBody.width,120.0f }, " ", &m_Config.gridWidth, 1, 1'000, m_VariableAMode) == 1) // width
			{
				m_VariableAMode = !m_VariableAMode;
			}

			GuiLabel(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 4 + 30,m_WinBoxBody.width,120.0f }, "Grid Height");
			if (GuiValueBox(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 5 + 30,m_WinBoxBody.width,120.0f }, " ", &m_Config.gridHeight, 1, 1'000, m_VariableBMode) == 1) //height
			{
				m_VariableBMode = !m_VariableBMode;
			}

			if (GuiButton(Rectangle{ m_WinBoxBody.x,m_WinBoxBody.y + 120 * 6 + 30,m_WinBoxBody.width,120.0f }, "Send Form") == 1)
			{
				if (!FilterSendForm())
				{
					ResetNewProjectConfig();
				}
				else {
					SendNewProjectConfig();
					m_SubmitNewProject = true;
				}
			}

		}
		else 
		{
			ResetNewProjectConfig();
			m_MenuState = MenuAction::NONE;
		}
	}

	void Resize()
	{
		m_WinBoxBody.x = (static_cast<float>(GetScreenWidth()) / 2.0f) - m_WinBoxBody.width / 2.0f;
		m_WinBoxBody.y = (static_cast<float>(GetScreenHeight()) * 5.0f / 100.0f);
	}
private:

	void SendNewProjectConfig() const
	{
		Core::Application::Get().GetSaveManager().createSave(m_Config.projectName, m_Config.gridWidth, m_Config.gridHeight);
	}

	void ResetNewProjectConfig()
	{
		m_Config.projectName = std::string(15, '\0');
		m_Config.gridWidth = 0;
		m_Config.gridHeight = 0;
	}

	bool FilterSendForm()
	{
		std::string cleanName = m_Config.projectName.c_str();

		if (!(std::regex_match(cleanName, m_Filter)))
		{
			std::println("Word -> {} does not match the requirements!", cleanName);
			return false;
		}
		if ((m_Config.gridWidth > 1'000) || (m_Config.gridWidth < 0) ||
			(m_Config.gridHeight > 1'000) || (m_Config.gridHeight < 0))
		{
			std::println("Grid Height or Grid Width too big or too small");
			return false;
		}

		m_Config.projectName = cleanName;
		return true;
	}

private:

	bool m_VariableAMode = false;
	bool m_VariableBMode = false;
	bool m_VariableCMode = false;

	bool m_SendFormFailed = false;
	bool& m_SubmitNewProject;

	MenuAction& m_MenuState;

	NewProjectConfig& m_Config;

	Rectangle m_WinBoxBody = { static_cast<float>(GetScreenWidth()) / 2.0f - 440.0f,50.0f, 440.0f,880.0f };

	std::regex m_Filter{ "[a-zA-Z][a-zA-Z0-9_]{1,15}" };
};


class OpenProject_UI
{
public:
	OpenProject_UI(MenuAction& btnState, bool& submitAction, std::filesystem::path& pathToSave)
		:
		m_MenuBtnState(btnState),
		m_SubmitAction(submitAction),
		m_PathToSave(pathToSave)
	{

	}

	~OpenProject_UI() = default;

	void update()
	{
		for (auto const& dir_entry : std::filesystem::directory_iterator{ Config::getSavesDir() })
		{
			m_PanelContent.height += 120.0f;
		}
		
		GuiScrollPanel(m_WinBoxBody, "Choose Save", m_PanelContent, &m_PanelScroll, &m_PanelView);

		if (GuiButton(Rectangle{ 10.0f, 10.0f, m_WinBoxBody.width * 40.0f/100 , m_WinBoxBody.height * 15.0f/100}, "Leave") == 1)
		{
			m_MenuBtnState = MenuAction::NONE;
			return;
		}

		BeginScissorMode(static_cast<int>(m_PanelView.x), static_cast<int>(m_PanelView.y), static_cast<int>(m_PanelView.width), static_cast<int>(m_PanelView.height));

		for (const auto [index, pathToSave] : std::views::enumerate(std::filesystem::directory_iterator{ Config::getSavesDir() }))
		{
			Rectangle itemRect = {
				m_WinBoxBody.x + m_PanelScroll.x + 20,				 // Base X + Scroll Offset + Padding
				m_WinBoxBody.y + m_PanelScroll.y + 20 + (index * 120), // Base Y + Scroll Offset + Padding + Spacing
				400,
				100
			};
			if (GuiButton(itemRect, pathToSave.path().stem().string().c_str()) == 1)
			{
				m_PathToSave = std::filesystem::path(pathToSave.path()).append(pathToSave.path().stem().string()+".db");
				m_SubmitAction = true;
				break;
			}
		}


		EndScissorMode(); 
		m_PanelContent.height = 0.0f;
	}

	void Resize() { 
		m_WinBoxBody.x = (static_cast<float>(GetScreenWidth()) / 2.0f) - m_WinBoxBody.width / 2.0f;
		m_WinBoxBody.y = (static_cast<float>(GetScreenHeight()) * 30.0f / 100.0f);
	
	}

private:

	Rectangle m_WinBoxBody = { 400.0f, 100.0f, 500.0f, 500.0f };//{ GetScreenWidth() / 2.0f - 440.0f,50.0f, 440.0f,880.0f };
	Rectangle m_PanelContent = { 0.0f, 0.0f, 500.0f, 0.0f };    // The total size of the inside area
	Vector2 m_PanelScroll = { 0.0f, 0.0f };                       // Tracks X/Y scroll position
	Rectangle m_PanelView = { 0 };

	MenuAction& m_MenuBtnState;
	bool& m_SubmitAction;
	std::filesystem::path& m_PathToSave;

	std::vector<std::filesystem::path> m_SavePaths;
};


class Menu
{
public:

	Menu() {
		m_ButtonH = static_cast<float>(GetScreenHeight()) * 10 / 100.0f;
		m_ButtonW = static_cast<float>(GetScreenWidth()) * 15 / 100.0f;
		m_MarginLeft = (static_cast<float>(GetScreenWidth()) / 2.0f) - m_ButtonW / 2.0f;
		m_MarginTop = static_cast<float>(GetScreenHeight()) * 25 / 100.0f;
		GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(m_ButtonH * 25.0f / 100));
	}

	void update()
	{
		switch (m_BtnState)
		{
		case MenuAction::NONE:
			drawMenu();
			drawTitle();
			break;
		case MenuAction::NEW_PROJECT:
			DrawRectangle(0.0f, 0.0f, GetScreenWidth(), GetScreenHeight(), Color{ 70,130,180,255 });
			m_NewProject.draw();
			break;
		case MenuAction::OPEN_PROJECT:
			m_OpenProject.update();
			break;
		case MenuAction::SAVE_PROJECT:
			m_SubmitAction = true;
			break;
		}
	}


	MenuAction& getMenuState()
	{
		return m_BtnState;
	}

	void ResetMenuState()
	{
		std::print("Reseting Menu State!");
		m_BtnState = MenuAction::NONE;
		m_SubmitAction = false;

		m_NewProjectConfig.projectName = std::string(15, '\0');
		m_NewProjectConfig.gridWidth = 1;
		m_NewProjectConfig.gridHeight = 1;

		m_PathToSave = "";
	}

	void resizeMenu()
	{
		m_ButtonH = static_cast<float>(GetScreenHeight()) * 10 / 100.0f;
		m_ButtonW = static_cast<float>(GetScreenWidth()) * 15 / 100.0f;
		m_MarginLeft = (static_cast<float>(GetScreenWidth()) / 2.0f) - m_ButtonW / 2.0f;
		m_MarginTop = static_cast<float>(GetScreenHeight()) * 25 / 100.0f;
		GuiSetStyle(DEFAULT, TEXT_SIZE, static_cast<int>(m_ButtonH * 25.0f / 100));
		m_OpenProject.Resize();
		m_NewProject.Resize();
	}


	bool ActionSubmited() const { return m_SubmitAction; } 

	std::filesystem::path GetPathToSave() { return m_PathToSave; }

	NewProjectConfig& GetNewProjectConfig() { return m_NewProjectConfig; }

private:

	void drawTitle()
	{
		DrawText("  Map Maker", (static_cast<float>(GetScreenWidth()) / 2.0f) - (static_cast<float>(GetScreenHeight()) * 0.3f), 55, static_cast<float>(GetScreenHeight()) * 0.1f, RAYWHITE);
		DrawText("Version 1.0", (static_cast<float>(GetScreenWidth()) / 2.0f) - (static_cast<float>(GetScreenHeight()) * 0.1f), 55.0f + static_cast<float>(GetScreenHeight()) * 0.1f, static_cast<float>(GetScreenHeight()) * 0.05f, RAYWHITE);
	}

	void drawMenu()
	{
		float boxSize = GetScreenHeight() * 0.05f;
		int iconScale = static_cast<int>(boxSize / 16.0f);

		if (iconScale < 1) iconScale = 1;

		GuiSetIconScale(iconScale);

		DrawRectangle(0.0f, 0.0f, GetScreenWidth(), GetScreenHeight(), Color{ 70,130,180,255 });
		if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop, m_ButtonW, m_ButtonH }, "#5#New Project") == 1)
		{
			m_BtnState = MenuAction::NEW_PROJECT;
		}
		if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop + m_ButtonH, m_ButtonW, m_ButtonH }, "#1#Open Project") == 1)
		{
			m_BtnState = MenuAction::OPEN_PROJECT;
		}
		if (GuiButton(Rectangle{ m_MarginLeft, m_MarginTop + (m_ButtonH * 2), m_ButtonW, m_ButtonH }, "#2#Save Project") == 1)
		{
			m_BtnState = MenuAction::SAVE_PROJECT;
		}

		GuiSetIconScale(1);

	}

private:
	float m_MarginLeft = 0.0f;
	float m_MarginTop = 0.0f;
	float m_ButtonH = 0.0f;
	float m_ButtonW = 0.0f;



	MenuAction m_BtnState = MenuAction::NONE;

	bool m_SubmitAction = false;

	NewProjectConfig m_NewProjectConfig;
	std::filesystem::path m_PathToSave;

	NewProject_UI m_NewProject{ m_BtnState,m_NewProjectConfig,m_SubmitAction };
	OpenProject_UI m_OpenProject{ m_BtnState,m_SubmitAction,m_PathToSave };
};

