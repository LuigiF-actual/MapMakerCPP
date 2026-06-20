#pragma once

#include <array>
#include <memory>
#include <print>

#include <raylib.h>
#include <raymath.h>

#include "TileGrid.hpp"
#include "Command.hpp"
#include "Config.hpp"
#include "KeyboardManager.hpp"
#include "AtlasManager.hpp"
#include "PaletteGrid.hpp"
#include "PaintMode.hpp"

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
		checkKeyboard();
		checkMouseInput();
	}

	bool& GetRedoBool() { return m_RedoCmd; }
	bool& GetUndoBool()  { return m_UndoCmd; }
	PaintMode& GetPmode() { return m_Mode; }

private:

	void checkKeyboard();
	void checkMouseInput();

	void rectangleFill();

	void paintTile();

	void pickPaletteCell();

private:

	TileGrid& m_TileGrid;
	TexturePalette& m_TexturesPallete;
	Camera2D& m_WorldCam;


	bool m_UndoCmd = false;
	bool m_RedoCmd = false;

	Vector2 m_Begin = { 0.0f, 0.0f };
	Vector2 m_End = { 0.0f, 0.0f };

	KeyboardManager m_Keyboard;

	Tile* m_SelectedCell = nullptr;

	CommandManager m_Cmd;

	PaintMode m_Mode = PaintMode::RECTANGLE;
};

