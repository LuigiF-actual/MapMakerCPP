#pragma once

#include "Core/SaveData.hpp"

#include <iostream>
#include <vector>
#include <print>

#include <raylib.h>

#include "AtlasManager.hpp"
#include "Config.hpp"
#include "TileDef.hpp"

class TileGrid
{
public:
	TileGrid(int columnNums, int rowNums, float tileSize, std::vector<Core::TileSaveData>& vec2Load)
		:
		m_Cols(columnNums),
		m_Rows(rowNums),
		m_TileSize(tileSize)
	{
		std::println("Here at constructior -> {} : {} ", columnNums, rowNums);

		for (int row = 0; row < m_Rows; row++)
		{
			for (int column = 0; column < m_Cols; column++)
			{
				m_TileGrid.emplace_back(
					Tile{
						&AtlasManager::getInstance().getTexture(vec2Load.at(row * m_Cols + column).spriteName),
						Rectangle
						{
							static_cast<float>(column) * m_TileSize,
							static_cast<float>(row) * m_TileSize,
							tileSize,
							tileSize
						},
						Rectangle
						{
							static_cast<float>(vec2Load.at(row * m_Cols + column).srcX),
							static_cast<float>(vec2Load.at(row * m_Cols + column).srcY),
							Config::paletteTilesSize,
							Config::paletteTilesSize
						}
					}

				);
			}
		}
	}


	TileGrid(int columnNums, int rowNums, float tileSize, Vector2 gridOffSet)
		:
		m_Cols(columnNums),
		m_Rows(rowNums),
		m_TileSize(tileSize),
		m_GridOffSet(gridOffSet)
	{

		for (int row = 0; row < m_Rows; row++)
		{
			for (int column = 0; column < m_Cols; column++)
			{
				m_TileGrid.emplace_back(
					Tile{
						&AtlasManager::getInstance().getTexture(Config::getDefault_Atlas()),
						Rectangle{
							static_cast<float>(column) * m_TileSize + m_GridOffSet.x,
							static_cast<float>(row) * m_TileSize + m_GridOffSet.y,
							m_TileSize,
							m_TileSize
						},
						Rectangle{0.0f, 0.0f, Config::paletteTilesSize, Config::paletteTilesSize}
					}
				);
			}
		}
	}

	TileGrid(int columnNums, int rowNums)
		:
		m_Cols(columnNums),
		m_Rows(rowNums)
	{
		for (int row = 0; row < m_Rows; row++)
		{
			for (int column = 0; column < m_Cols; column++)
			{
				m_TileGrid.emplace_back(
					Tile{
						&AtlasManager::getInstance().getTexture(Config::getDefault_Atlas()),
						Rectangle{
							static_cast<float>(column) * m_TileSize,
							static_cast<float>(row) * m_TileSize,
							Config::tileSize,
							Config::tileSize
						},
						Rectangle{0.0f, 0.0f, Config::paletteTilesSize, Config::paletteTilesSize}
					}
				);
			}
		}
	}

	std::vector<Core::TileSaveData> DataToBeSaved() const
	{
		std::vector<Core::TileSaveData> data;
		data.reserve(m_TileGrid.size());

		for (const auto& tile : m_TileGrid)
		{
			data.emplace_back(Core::TileSaveData{
				AtlasManager::getInstance().getTextureName(tile.texture),
				static_cast<int>(tile.scRec.x),
				static_cast<int>(tile.scRec.y)
			});
		}

		return data;
	}

	Tile* at(int column, int row)
	{
		if (column < 0 || column >= m_Cols || row < 0 || row >= m_Rows) {
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
		for (auto& tile : m_TileGrid)
		{
			if (CheckCollisionPointRec(position, tile.body))
			{
				return &tile;
			}
		}
		return nullptr;
	}

	[[nodiscard]] Vector2 getGridSize() const
	{
		return Vector2{ static_cast<float>(m_Cols), static_cast<float>(m_Rows) };
	}

	void setGidPos(Vector2 newPos)
	{
		for (int row = 0; row < m_Rows; row++)
		{
			for (int column = 0; column < m_Cols; column++)
			{
				size_t index = (static_cast<std::size_t>(row) * m_Cols) + column;

				m_TileGrid[index].body.x = static_cast<float>(column) * m_TileSize + newPos.x;
				m_TileGrid[index].body.y = static_cast<float>(row) * m_TileSize + newPos.y;
			}
		}
	}

	void resetScRecs()
	{
		for (int row = 0; row < m_Rows; row++)
		{
			for (int column = 0; column < m_Cols; column++)
			{
				size_t index = (static_cast<std::size_t>(row) * m_Cols) + column;

				m_TileGrid[index].scRec.x = static_cast<float>(column) * m_TileSize;
				m_TileGrid[index].scRec.y = static_cast<float>(row) * m_TileSize;
			}
		}
	}

	[[nodiscard]] Vector2 offset() const { return m_GridOffSet; }

	[[nodiscard]] int cols() const { return m_Cols; }

	[[nodiscard]] int rows() const { return m_Rows; }

private:

	int m_Cols = 1;
	int m_Rows = 1;

	float m_TileSize = Config::tileSize;

	Vector2 m_GridOffSet = {0.0f,0.0f};

	std::vector<Tile> m_TileGrid;
};


class TileRenderer
{
public:
	TileRenderer() = default;

	static void draw(TileGrid& mp_GridToDraw, Camera2D& camera)
	{
		Vector2 windowStart = GetScreenToWorld2D(mp_GridToDraw.offset(), camera);
		Vector2 windowEnd = GetScreenToWorld2D(Vector2{ static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight()) }, camera);

		for (auto yPos = static_cast<int>(windowStart.y / Config::tileSize); yPos < static_cast<int>(windowEnd.y / Config::tileSize) + 1; yPos++)
		{
			for (auto xPos = static_cast<int>(windowStart.x / Config::tileSize); xPos < static_cast<int>(windowEnd.x / Config::tileSize) + Config::tileSize; xPos++)
			{
				if (!(xPos < 0 || xPos >= mp_GridToDraw.cols() || yPos < 0 || yPos >= mp_GridToDraw.rows()))
				{

					Tile* tile = mp_GridToDraw.at(xPos, yPos);
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

	static void drawRecLines(TileGrid& mp_GridToDraw)
	{
		for (auto& tile : mp_GridToDraw.getArr())
		{
			DrawRectangleLinesEx(tile.body, 1.0f, tile.borderColor);
		}
	}

private:


};
