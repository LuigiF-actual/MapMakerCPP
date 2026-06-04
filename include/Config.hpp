#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <string_view>

namespace Config
{
	constexpr int screenWidth = 1600;
	constexpr int screenHeight = 800;


	constexpr int textureTileSize = 16;
	constexpr float tileSize = 64.0f;
	constexpr int paletteTextureScale = 3;
	constexpr int paletteTilesSize = 16;


	constexpr float worldCamMoveSpd = 250.0f;

	constexpr float zoomSpd = 0.1f;


	const std::string default_Atlas = "GrassComplete";


	//inline const std::filesystem::path imagesDir = std::filesystem::path(RESOURCES_PATH).lexically_normal();
	//inline const std::filesystem::path savesDir = std::filesystem::path(SAVES_PATH).lexically_normal();


	//Obligates the user to use the return value in some way.
	[[nodiscard]] inline const std::filesystem::path& getSavesDir() {

		static const std::filesystem::path savesDir = std::filesystem::path(SAVES_PATH).lexically_normal();

		return savesDir;
	}


	[[nodiscard]] inline const std::filesystem::path& getImagesDir() {
		// New way to have it initialized only when a variable in the game needs it
		static const std::filesystem::path imagesDir = std::filesystem::path(RESOURCES_PATH).lexically_normal();

		return imagesDir;
	}
}