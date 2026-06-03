#pragma once

#include <string>
#include <filesystem>
#include <tinyfiledialogs.h>
#include <Config.hpp>

class FileExplorer
{
public:
	FileExplorer()
	{
		std::cout << Config::imagesDir.string().c_str() << "\n";
	}

	std::string openExplorer()
	{
		const char* Path = tinyfd_openFileDialog( 
			"Select Texture",
			Config::imagesDir.string().c_str(),
			2,
			m_FiltersForFolder,
			"images files",
			0
		);

		if(Path == nullptr)
		{
			return "";
		}

		return std::string(Path);
	}


	std::string openExplorer(const std::string& path)
	{
		const char* Path = tinyfd_openFileDialog(
			"Select Texture",
			path.c_str(),
			2,
			m_FiltersForFolder,
			"images files",
			0
		);

		if (Path == nullptr)
		{
			return "";
		}

		return std::string(Path);
	}

	std::string saveFile()
	{
		const char* Path = tinyfd_saveFileDialog(
			"Save file",
			Config::savesDir.string().c_str(),
			1,
			m_FilterForSaves,
			"db files"
		);

		if (Path == nullptr)
		{
			return "";
		}
		
		return std::string(Path);
	}


private:
	const char* m_FiltersForFolder[2] = { "*.jpg","*.png" };
	const char* m_FilterForSaves[1] = { "*.db" };
};
