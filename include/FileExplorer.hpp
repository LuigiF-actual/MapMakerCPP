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
			filtersForFolder,
			"images files",
			0
		);

		if(Path)
		{
			return std::string(Path);
		}
		else
		{
			return "";
		}
	}


	std::string openExplorer(std::string_view path)
	{
		const char* Path = tinyfd_openFileDialog(
			"Select Texture",
			path.data(),
			2,
			filtersForFolder,
			"images files",
			0
		);

		if (Path)
		{
			return std::string(Path);
		}
		else
		{
			return "";
		}
	}

	std::string saveFile()
	{
		const char* Path = tinyfd_saveFileDialog(
			"Save file",
			Config::savesDir.string().c_str(),
			1,
			filterForSaves,
			"db files"
		);

		if (Path)
		{
			return std::string(Path);
		}
		else
		{
			return "";
		}
	}


private:
	const char* filtersForFolder[2] = { "*.jpg","*.png" };
	const char* filterForSaves[1] = { "*.db" };
};
