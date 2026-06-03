#pragma once 

#include <string>
#include <string_view>
#include <filesystem>
#include <fstream>

#include "Menu.hpp"

#include "Config.hpp"

class SavesFilesManager
{
public:
	SavesFilesManager(Menu menu)
		:
		m_Menu(menu)
	{};

	void createSave(std::string_view saveName)
	{

		std::filesystem::path pathToSave = std::filesystem::path(Config::savesDir).append(saveName);
		if (!std::filesystem::is_directory(pathToSave.string()))
		{

			std::filesystem::create_directory(pathToSave);
			
			std::ofstream Files(std::filesystem::path(pathToSave).append(std::string(saveName)+".db"));
			Files.close();

			Files.open(std::filesystem::path(pathToSave).append(std::string(saveName)+".json"));
			Files.close();



		}
	}

private:
	std::string m_CurrentSave;
	Menu& m_Menu;
};

