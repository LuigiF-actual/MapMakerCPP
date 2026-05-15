#pragma once

#include <iostream>
#include <filesystem>

namespace Config
{
	constexpr int screenWidth = 1600;
	constexpr int screenHeight = 800;


	constexpr int textureTileSize = 16;
	constexpr float tileSize = 32;
	constexpr int paletteTextureScale = 3;
	constexpr int paletteTilesSize = 16;

	inline const std::filesystem::path imagesDir = std::filesystem::path(RESOURCES_PATH).lexically_normal().string();
}