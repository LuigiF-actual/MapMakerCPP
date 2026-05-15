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
				if ((m_PlMouse.getMousePos().y < m_TexturesPallete.getTileArr().at(0).body.y) && (m_CurrentTexture))
				{
					Tile* p_Tile = m_TileGrid.findTile(m_PlMouse.getMousePos());
					if (p_Tile)
					{
						cmd.execute(*p_Tile, m_CurrentSourceRec, m_CurrentTexture);
					}
				}
				else
				{
					for (Tile& tile : m_TexturesPallete.getTileArr())
					{
						if (CheckCollisionPointRec(GetScreenToWorld2D(GetMousePosition(), m_paletteCam), tile.body))
						{
							m_CurrentTexture = &m_TexturesPallete.getTexture();
							
							if ((tile.scRec.x > m_CurrentTexture->width - Config::paletteTilesSize) || (tile.scRec.y > m_CurrentTexture->height - Config::paletteTilesSize))
							{

								std::cout << "Calling break\n";
								break;
							}
							
							if (!m_SelectedCell)
							{
								m_SelectedCell = &tile;
								m_SelectedCell->borderColor = ORANGE;
							}

							if (!ColorIsEqual(m_SelectedCell->borderColor, tile.borderColor))
							{
								m_SelectedCell->borderColor = BLACK;
								m_SelectedCell = &tile;
								m_SelectedCell->borderColor = ORANGE;
							}
							m_CurrentSourceRec = {
								tile.scRec.x,
								tile.scRec.y,
								16,
								16
							};

							break;
						}
					}
				}
		}
	}

private:

	TileGrid& m_TileGrid;
	TexturePalette& m_TexturesPallete;
	Camera2D& m_paletteCam;
	PlayerMouse m_PlMouse;
	KeyboardManager keyboard;

	const Texture2D* m_CurrentTexture = nullptr;
	Rectangle m_CurrentSourceRec = { 0,0,0.0f,0.0f };
	Tile* m_SelectedCell = nullptr;


	CommandManager cmd;
};
