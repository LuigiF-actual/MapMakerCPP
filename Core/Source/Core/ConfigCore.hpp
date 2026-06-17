#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>


namespace Core
{

	[[nodiscard]] inline const std::string& getDefault_Atlas() {

		static const std::string default_Atlas = "GrassComplete";

		return default_Atlas;
	}

	//Obligates the user to use the return value in some way.
	[[nodiscard]] inline const std::filesystem::path& getSavesDir() {

		static const std::filesystem::path savesDir = std::filesystem::path(SAVES_PATH).lexically_normal();

		return savesDir;
	}


	[[nodiscard]] inline const std::filesystem::path& getImagesDir() {
		// New way to have it initialized only when a variable in the game needs it
		static const std::filesystem::path imagesDir = std::filesystem::path(TEXTURES_PATH).lexically_normal();

		return imagesDir;
	}
}