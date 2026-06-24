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
		std::cout << Config::getImagesDir().string().c_str() << "\n";
	}

	std::string openExplorer() const
	{
		const char* Path = tinyfd_openFileDialog(
			"Select Texture",
			(Config::getImagesDir().string() + "/").c_str(),
			2,
			m_FiltersForFolder,
			"images files",
			0
		);

		if (Path == nullptr)
		{
			return "";
		}


		return { Path };
	}


	std::string openExplorerEx(const std::string& path) const
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

		return { Path };
	}

	std::string saveFile() const
	{
		const char* Path = tinyfd_saveFileDialog(
			"Save file",
			Config::getSavesDir().string().c_str(),
			1,
			m_FilterForSaves,
			"db files"
		);

		if (Path == nullptr)
		{
			return "";
		}

		return { Path };
	}


private:
	const char* m_FiltersForFolder[2] = { "*.png", "*.jpg" };
	const char* m_FilterForSaves[1] = { "*.db" };
};
