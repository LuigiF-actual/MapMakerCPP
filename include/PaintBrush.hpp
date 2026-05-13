#pragma once

#include <iostream>
#include <array>

#include <raylib.h>
#include "TileGrid.hpp"
#include "PlayerMouse.hpp"
#include "Command.hpp"
#include "Config.hpp"
#include "KeyboardManager.hpp"
#include "AtlasDictionary.hpp"
#include "TexturePalette.hpp"

class PaintBrush
{

public:
	PaintBrush(TileGrid& tileGrid,TexturePalette& palette)
		: 
		m_TileGrid(tileGrid),
		m_TexturesPallete(palette)
	{
	}


	void update()
	{
		checkMouseInput();
		checkKeyboard();
	}


	void checkMouseInput()
	{
		if ((m_PlMouse.isMouseClicked()) && (m_PlMouse.getMousePos().x < m_TexturesPallete.getTileArr().at(0).body.x))
		{
			Tile* p_Tile = m_TileGrid.findTile(m_PlMouse.getMousePos());
			if (p_Tile)
			{
				cmd.execute(*p_Tile, m_CurrentTexture);
			}
		}
		else
		{
			for (Tile& tile : m_TexturesPallete.getTileArr())
			{
				if (CheckCollisionPointRec(m_PlMouse.getMousePos(), tile.body))
				{
					m_CurrentTexture = tile.texture;
					break;
				}
			}
		}
	}

	void checkKeyboard()
	{
		if (keyboard.ctrlZ())
		{
			cmd.undo();
		}
		if (keyboard.ctrlY())
		{
			cmd.redo();
		}
	}

private:
	PlayerMouse m_PlMouse;
	KeyboardManager keyboard;

	const Texture2D* m_CurrentTexture = { 0 };

	TileGrid& m_TileGrid;
	TexturePalette& m_TexturesPallete;

	CommandManager cmd;
};
