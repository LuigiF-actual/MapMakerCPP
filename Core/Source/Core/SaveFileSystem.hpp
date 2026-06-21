#pragma once 

#include <stdexcept>
#include <string>
#include <string_view>
#include <fstream>
#include <ranges>
#include <raylib.h>
#include <filesystem>
#include <span>
#include <print>

#include "DataBaseManager.hpp"

class SavesFilesManager
{
public:
	SavesFilesManager() = default;

	void createSave(std::string_view saveName,int GridW,int GridH)
	{

		std::filesystem::path pathToSave = std::filesystem::path(Core::getSavesDir()).append(saveName);
		if (!std::filesystem::is_directory(pathToSave.string()))
		{
			m_PathToCurrentSave = pathToSave;
			m_CurrentSave = saveName;

			std::filesystem::create_directory(pathToSave);
			createFile(pathToSave, saveName, ".db");

			m_DBmanager.ConnectToDB(std::filesystem::path(pathToSave).append(saveName).string().append(".db"));
			m_DBmanager.SetupNewDB(GridW, GridH);
			
		}
	}

	void SaveGame(std::span<Core::TileSaveData> tiles)
	{
		if (m_CurrentSave.empty())
		{
			return;
		}
		std::print("At SaveGame!\n");
		m_DBmanager.SaveGameState(tiles);
	}

	Core::SaveData LoadSave(const std::filesystem::path& pathToSave)
	{
		std::print("At LoadSave");
		m_DBmanager.ConnectToDB(pathToSave.string());

		m_PathToCurrentSave = pathToSave;
		m_CurrentSave = std::filesystem::path(pathToSave).stem().string();
		
		std::print("At return Load Save");
		return m_DBmanager.LoadGameSave();
	}


private:

	void createFile(std::filesystem::path filePath, const std::string_view fileName, const std::string_view suffix)
	{
		std::ofstream File(filePath.append(std::string(fileName) + std::string(suffix)));
		File.close();
	}

private:

	std::string m_CurrentSave;
	std::filesystem::path m_PathToCurrentSave;
	DataBaseManager m_DBmanager;
};
