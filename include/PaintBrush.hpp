#pragma once

#include <array>
#include <memory>

#include <raylib.h>
#include <raymath.h>

#include "TileGrid.hpp"
#include "PlayerMouse.hpp"
#include "Command.hpp"
#include "Config.hpp"
#include "KeyboardManager.hpp"
#include "AtlasDictionary.hpp"
#include "TexturePalette.hpp"


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
	PaintBrush(TileGrid& tileGrid,TexturePalette& palette,Camera2D& paletteCamera,Camera2D& worldCamera)
		: 
		m_TileGrid(tileGrid),
		m_TexturesPallete(palette),
		m_paletteCam(paletteCamera),
		m_worldCam(worldCamera)
	{
	}

	void update()
	{
		checkMouseInput();
		checkKeyboard();
	}


private:

	void checkKeyboard()
	{
		int key = GetKeyPressed();

		switch (key)
		{
		case KEY_B:
			mode = PaintMode::NORMAL;
			break;
		case KEY_U:
			mode = PaintMode::RECTANGLE;
			break;
		}

		if (keyboard.ctrlZ())
		{
			cmd.undo();
			
		}
		if (keyboard.ctrlY())
		{
			cmd.redo();
		}
	}


	void checkMouseInput()
	{

		if (GetMousePosition().y > m_TexturesPallete.getPosition().y)
		{
			pickPaletteCell();
		}
		else 
		{
			switch (mode)
			{
			case PaintMode::RECTANGLE:
				rectangleFill();
				break;
			case PaintMode::NORMAL:
				paintTile();
				break;
			}
		}
	}

private:

	void rectangleFill()
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			begin = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_worldCam), m_TileGrid.offset());
			std::cout << "Begin " << begin.x << " : " << begin.y << "\n";
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && m_SelectedCell)
		{
			end = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_worldCam), m_TileGrid.offset());

			std::cout << "end " << end.x << " : " << end.y << "\n";

			float beginX = int(std::min(begin.x, end.x) / Config::tileSize);
			float beginY = int(std::min(begin.y, end.y) / Config::tileSize) ;

			float endX = int(std::max(begin.x, end.x) / Config::tileSize);
			float endY = int(std::max(begin.y, end.y) / Config::tileSize);

			Tile* tileOld = m_TileGrid.at(beginX, beginY);

			auto fillRec = std::make_unique<RectangleFillCmd>(m_TileGrid, Vector2{ beginX,beginY }, Vector2{ endX, endY }, m_SelectedCell->scRec, &m_TexturesPallete.getTexture());
			cmd.execute(std::move(fillRec));

			begin = { 0.0f };
			end = { 0.0f };
		}
	}

	void paintTile()
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Tile* p_Tile = m_TileGrid.findTile(GetScreenToWorld2D(m_PlMouse.getMousePos(), m_worldCam));

			if (p_Tile)
			{	
				//Checks to see if the tile texture is not the same that it will be aplied, if it is it will not be applied
				if (!((p_Tile->scRec.x == m_SelectedCell->scRec.x) && (p_Tile->scRec.y == m_SelectedCell->scRec.y) && (&m_TexturesPallete.getTexture() == p_Tile->texture)))
				{
					auto paintCmd = std::make_unique<PaintTileCmd>(*p_Tile, m_SelectedCell->scRec, &m_TexturesPallete.getTexture());
					cmd.execute(std::move(paintCmd));
				}
			}
		}
	}

	void pickPaletteCell()
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			m_SelectedCell = m_TexturesPallete.getSelectedCell();
		}
	}

private:

	Vector2 begin = { 0 };
	Vector2 end = { 0 };


	TileGrid& m_TileGrid;
	TexturePalette& m_TexturesPallete;
	Camera2D& m_paletteCam;
	Camera2D& m_worldCam;

	PlayerMouse m_PlMouse;
	KeyboardManager keyboard;

	Tile* m_SelectedCell = nullptr;


	CommandManager cmd;

	PaintMode mode = PaintMode::RECTANGLE;
};

/*if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			begin = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_worldCam), m_TileGrid.offset());
			m_SelectedCell = m_TexturesPallete.getSelectedCell();
			std::cout << "Begin " << begin.x << " : " << begin.y << "\n";
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && m_SelectedCell)
		{
			end = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_worldCam), m_TileGrid.offset());

			std::cout << "end " << end.x << " : " << end.y << "\n";

			float beginX = int(std::min(begin.x, end.x));
			float beginY = int(std::min(begin.y, end.y));

			float endX = int(std::max(begin.x, end.x));
			float endY = int(std::max(begin.y, end.y));

			begin = { 0.0f };
			end = { 0.0f };
}*/