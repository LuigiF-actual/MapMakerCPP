#pragma once

#include <iostream>
#include <vector>

#include <raylib.h>

#include "AtlasDictionary.hpp"

struct Tile
{
	const Texture2D* texture;
	Rectangle body;
	Rectangle scRec;
};

class TileGrid
{
public:
	TileGrid(size_t columnNums, size_t rowNums, float tileSize, Vector2 gridOffSet)
		: 
		m_GridW(columnNums),
		m_GridH(rowNums),
		m_TileSize(tileSize),
		m_GridOffSet(gridOffSet)
	{

		for (size_t row = 0; row < m_GridH; row++)
		{
			for (size_t column = 0; column < m_GridW; column++)
			{
				m_TileGrid.emplace_back(
					Tile{
						&AtlasManager::getInstance().getFirstTexture(),
						Rectangle{
							(float)column * m_TileSize + m_GridOffSet.x,
							(float)row * m_TileSize + m_GridOffSet.y,
							m_TileSize,
							m_TileSize
						},
						Rectangle{0.0f, 0.0f, 16.0f, 16.0f}
					}
				);
			}
		}
	}

	Tile* at(unsigned int column, unsigned int row)
	{
		if (column >= m_GridW || row >= m_GridH) {
			return nullptr;
		}
		return &m_TileGrid[row * m_GridW + column];
	}

	auto& getArr()
	{
		return m_TileGrid;
	}

	Tile* findTile(Vector2 position)
	{
		for (auto& tile : m_TileGrid)
		{
			if (CheckCollisionPointRec(position, tile.body))
			{
				return &tile;
			}
		}
		return nullptr;
	}
	

private:


	size_t m_GridW;
	size_t m_GridH;
	
	float m_TileSize = 100;

	Vector2 m_GridOffSet;

	std::vector<Tile> m_TileGrid;
};


class TileRenderer
{
public:
	TileRenderer() = default;

	void draw(TileGrid& mp_GridToDraw) const
	{
		for (auto& tile : mp_GridToDraw.getArr())
		{
			DrawTexturePro(
				*tile.texture,
				tile.scRec,
				tile.body,
				{ 0,0 },
				0.0f,
				WHITE
			);
			DrawRectangleLinesEx(tile.body, 1.0f, BLACK);
		}
	}

	void drawRecLines(TileGrid& mp_GridToDraw) const
	{
		for (auto& tile : mp_GridToDraw.getArr())
		{
			DrawRectangleLinesEx(tile.body, 1.0f, BLACK);
		}
	}

};
