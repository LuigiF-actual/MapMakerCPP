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
			"TESTING",
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

private:
	const char* filtersForFolder[2] = { "*.jpg","*.png" };
};
