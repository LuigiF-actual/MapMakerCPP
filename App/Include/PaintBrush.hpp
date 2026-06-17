#pragma once

#include <array>
#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "TileGrid.hpp"
#include "Command.hpp"
#include "Config.hpp"
#include "KeyboardManager.hpp"
#include "AtlasManager.hpp"
#include "PaletteGrid.hpp"


enum class PaintMode : unsigned char
{
	NONE,
	NORMAL,
	RECTANGLE,
	FILL,
	PICKCOLOR
};


class PaintBrush
{

public:
	PaintBrush(TileGrid& tileGrid, TexturePalette& palette, Camera2D& m_WorldCam)
		:
		m_TileGrid(tileGrid),
		m_TexturesPallete(palette),
		m_WorldCam(m_WorldCam)
	{}

	void update()
	{
		checkMouseInput();
		checkKeyboard();
	}


private:

	void checkKeyboard();
	void checkMouseInput();

private:

	void rectangleFill();

	void paintTile();

	void pickPaletteCell();

private:

	Vector2 m_Begin = { 0.0f, 0.0f };
	Vector2 m_End = { 0.0f, 0.0f };


	TileGrid& m_TileGrid;
	TexturePalette& m_TexturesPallete;
	Camera2D& m_WorldCam;

	KeyboardManager m_Keyboard;

	Tile* m_SelectedCell = nullptr;


	CommandManager m_Cmd;

	PaintMode m_Mode = PaintMode::RECTANGLE;
};

