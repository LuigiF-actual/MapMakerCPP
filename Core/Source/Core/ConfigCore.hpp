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

	[[nodiscard]] inline const std::filesystem::path& getSavesDir() {

		static const std::filesystem::path savesDir = std::filesystem::path(SAVES_PATH).lexically_normal();

		return savesDir;
	}


	[[nodiscard]] inline const std::filesystem::path& getImagesDir() {

		static const std::filesystem::path imagesDir = std::filesystem::path(TEXTURES_PATH).lexically_normal();

		return imagesDir;
	}
}