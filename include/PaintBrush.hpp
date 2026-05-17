#pragma once

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
	PaintBrush(TileGrid& tileGrid,TexturePalette& palette,Camera2D& paletteCamera)
		: 
		m_TileGrid(tileGrid),
		m_TexturesPallete(palette),
		m_paletteCam(paletteCamera)
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
		if ((m_PlMouse.isMousePressed()))
		{
				if ((m_PlMouse.getMousePos().y < m_TexturesPallete.getTileArr().at(0).body.y) && (m_SelectedCell))
				{
					Tile* p_Tile = m_TileGrid.findTile(m_PlMouse.getMousePos());
					if (p_Tile)
					{
						cmd.execute(*p_Tile, m_SelectedCell->scRec,&m_TexturesPallete.getTexture());
					}
				}
				else
				{
					m_SelectedCell = m_TexturesPallete.getSelectedCell();
				}
		}
	}

private:

	TileGrid& m_TileGrid;
	TexturePalette& m_TexturesPallete;
	Camera2D& m_paletteCam;
	PlayerMouse m_PlMouse;
	KeyboardManager keyboard;

	Tile* m_SelectedCell = nullptr;


	CommandManager cmd;
};
