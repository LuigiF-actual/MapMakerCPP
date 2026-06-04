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
	PaintBrush(TileGrid& tileGrid,TexturePalette& palette,Camera2D& worldCamera)
		: 
		m_TileGrid(tileGrid),
		m_TexturesPallete(palette),
		m_WorldCam(worldCamera)
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
			m_Mode = PaintMode::NORMAL;
			break;
		case KEY_U:
			m_Mode = PaintMode::RECTANGLE;
			break;
		default:
			break;
		}

		if (m_Keyboard.ctrlZ())
		{
			m_Cmd.undo();
			
		}
		if (m_Keyboard.ctrlY())
		{
			m_Cmd.redo();
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
			switch (m_Mode)
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
			m_Begin = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_WorldCam), m_TileGrid.offset());
		}
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && (m_SelectedCell != nullptr))
		{
			m_End = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_WorldCam), m_TileGrid.offset());


			float beginX = static_cast<float>(std::min(m_Begin.x, m_End.x) / Config::tileSize);
			float beginY = static_cast<float>(std::min(m_Begin.y, m_End.y) / Config::tileSize) ;

			float endX = static_cast<float>(std::max(m_Begin.x, m_End.x) / Config::tileSize);
			float endY = static_cast<float>(std::max(m_Begin.y, m_End.y) / Config::tileSize);

			auto fillRec = std::make_unique<RectangleFillCmd>(m_TileGrid, Vector2{ beginX,beginY }, Vector2{ endX, endY }, m_SelectedCell->scRec, &m_TexturesPallete.getTexture());
			m_Cmd.execute(std::move(fillRec));

			m_Begin = { 0.0f,0.0f };
			m_End = { 0.0f,0.0f };
		}
	}

	void paintTile()
	{
		if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		{
			Tile* p_Tile = m_TileGrid.findTile(GetScreenToWorld2D(m_PlMouse.getMousePos(), m_WorldCam));

			if ((p_Tile != nullptr) && (m_SelectedCell != nullptr))
			{	
				//Checks to see if the tile texture is not the same that it will be aplied, if it is it will not be applied
				if (p_Tile->scRec.x != m_SelectedCell->scRec.x ||
					p_Tile->scRec.y != m_SelectedCell->scRec.y ||
					&m_TexturesPallete.getTexture() != p_Tile->texture)
				{
					auto paintCmd = std::make_unique<PaintTileCmd>(*p_Tile, m_SelectedCell->scRec, &m_TexturesPallete.getTexture());
					m_Cmd.execute(std::move(paintCmd));
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

	Vector2 m_Begin = { 0.0f, 0.0f };
	Vector2 m_End = { 0.0f, 0.0f };


	TileGrid& m_TileGrid;
	TexturePalette& m_TexturesPallete;
	Camera2D& m_WorldCam;

	PlayerMouse m_PlMouse;
	KeyboardManager m_Keyboard;

	Tile* m_SelectedCell = nullptr;


	CommandManager m_Cmd;

	PaintMode m_Mode = PaintMode::RECTANGLE;
};

