#pragma once

#include <iostream>
#include <vector>

#include <raylib.h>

#include "AtlasDictionary.hpp"
#include "Config.hpp"

struct Tile
{
	const Texture2D* texture;
	Rectangle body;
	Rectangle scRec;
	Color borderColor = BLACK;
};

class TileGrid
{
public:
	TileGrid(size_t columnNums, size_t rowNums, float tileSize, Vector2 gridOffSet)
		: 
		m_Cols(columnNums),
		m_Rows(rowNums),
		m_TileSize(tileSize),
		m_GridOffSet(gridOffSet)
	{

		for (size_t row = 0; row < m_Rows; row++)
		{
			for (size_t column = 0; column < m_Cols; column++)
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

	Tile* at(int column,int row)
	{
		if (column >= m_Cols || row >= m_Rows) {
			return nullptr;
		}
		return &m_TileGrid[row * m_Cols + column];
	}

	auto& getArr()
	{
		return m_TileGrid;
	}

	Tile* findTile(Vector2 position)
	{
		std::cout << position.x << " :: " << position.y << "\n";
		for (auto& tile : m_TileGrid)
		{
			if (CheckCollisionPointRec(position, tile.body))
			{
				return &tile; 
			}
		}
		return nullptr;
	}

	Vector2 getGridSize()
	{
		return Vector2{ (float)m_Cols, (float)m_Rows };
	}
	
	void setGidPos(Vector2 newPos)
	{
		for (size_t row = 0; row < m_Rows; row++)
		{
			for (size_t column = 0; column < m_Cols; column++)
			{
				m_TileGrid[row * m_Cols + column].body.x = column * m_TileSize + newPos.x;
				m_TileGrid[row * m_Cols + column].body.y = row * m_TileSize + newPos.y;
			}
		}
	}

	void resetScRecs()
	{
		for (size_t row = 0; row < m_Rows; row++)
		{
			for (size_t column = 0; column < m_Cols; column++)
			{
				m_TileGrid[row * m_Cols + column].scRec.x = column * m_TileSize;
				m_TileGrid[row * m_Cols + column].scRec.y = row * m_TileSize;
			}
		}
	}


	int cols() const { return m_Cols; }

	int rows() const { return m_Rows; }

private:

	int m_Cols, m_Rows;
	
	float m_TileSize = 32.0f;

	Vector2 m_GridOffSet;

	std::vector<Tile> m_TileGrid;
};


class TileRenderer
{
public:
	TileRenderer() = default;

	void draw(TileGrid& mp_GridToDraw, Camera2D& camera) const
	{
		Vector2 windowStart = GetScreenToWorld2D(Vector2{ 0,0 }, camera);
		Vector2 windowEnd = GetScreenToWorld2D(Vector2{ float(GetScreenWidth()), float(GetScreenHeight()) }, camera);

		for (auto y = int(windowStart.y / Config::tileSize); y < int(windowEnd.y / Config::tileSize) + 1; y++)
		{
			for (auto x = int(windowStart.x / Config::tileSize); x < int(windowEnd.x / Config::tileSize) + 1; x++)
			{
				if ( ! (x < 0 || x >= (int)mp_GridToDraw.getGridSize().x || y < 0 || y >= (int)mp_GridToDraw.getGridSize().y))
				{

					Tile* tile = mp_GridToDraw.at(x, y);
					DrawTexturePro(
						*tile->texture,
						tile->scRec,
						tile->body,
						{ 0,0 },
						0.0f,
						WHITE
					);

				}
			}
		}

	}

	void drawRecLines(TileGrid& mp_GridToDraw) const
	{
		for (auto& tile : mp_GridToDraw.getArr())
		{
			DrawRectangleLinesEx(tile.body, 1.0f, tile.borderColor);
		}
	}

private:


};
