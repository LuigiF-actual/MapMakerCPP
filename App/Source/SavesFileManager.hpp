#pragma once 

#include <stdexcept>
#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <raylib.h>

#include "Menu.hpp"

#include "Config.hpp"

class SavesFilesManager
{
public:
	SavesFilesManager(Menu& menu)
		:
		m_Menu(menu)
	{};

	void createSave(std::string_view saveName)
	{

		std::filesystem::path pathToSave = std::filesystem::path(Config::getSavesDir()).append(saveName);
		if (!std::filesystem::is_directory(pathToSave.string()))
		{

			std::filesystem::create_directory(pathToSave);
			createFile(pathToSave, saveName,".json");
			createFile(pathToSave, saveName,".db");
		}
	}

	void update()
	{
		switch (m_Menu.getMenuState())
		{
		case MenuAction::SEND_FORM:
			break;
		}
	}

private:

	void createFile(std::filesystem::path filePath, const std::string_view fileName,const std::string_view suffix)
	{
		std::ofstream File(filePath.append(std::string(fileName) + std::string(suffix)));
		File.close();
	}

private:
	std::string m_CurrentSave;
	Menu& m_Menu;
};


//std::ofstream Files(std::filesystem::path(pathToSave).append(std::string(saveName)+".db"));
//Files.close();
//
//Files.open(std::filesystem::path(pathToSave).append(std::string(saveName)+".json"));
//Files.close();