#pragma once

#include <stack>
#include <memory>
#include <vector>
#include <queue>

#include <raylib.h>
#include "TileGrid.hpp"
#include "AtlasManager.hpp"
#include "Config.hpp"


class Command
{
public:
	virtual ~Command() = default;
	virtual void execute() = 0;
	virtual void undo() = 0;
};

class PaintTileCmd : public Command
{
public:
	PaintTileCmd(Tile& tile, Rectangle newSourceRec, const Texture2D* newTexture)
		:
		m_OriginalTexture(tile.texture),
		m_NewTexture(newTexture),
		m_NewSouceRec(newSourceRec),
		m_OriginalSourceRec(tile.scRec),
		m_PchangedTile(&tile)
	{}

	void execute() override
	{
		if (m_PchangedTile != nullptr)
		{
			m_PchangedTile->texture = m_NewTexture;
			m_PchangedTile->scRec = m_NewSouceRec;
		}
	}

	void undo() override
	{
		if (m_PchangedTile != nullptr)
		{
			m_PchangedTile->texture = m_OriginalTexture;
			m_PchangedTile->scRec = m_OriginalSourceRec;
		}
	}
private:
	const Texture2D* m_OriginalTexture = nullptr;
	const Texture2D* m_NewTexture = nullptr;

	Rectangle m_NewSouceRec = { 0.0f, 0.0f, 0.0f, 0.0f };
	Rectangle m_OriginalSourceRec = { 0.0f, 0.0f, 0.0f, 0.0f };

	Tile* m_PchangedTile = nullptr;
};

class RectangleFillCmd : public Command
{
public:
	RectangleFillCmd(TileGrid& TileVec, Vector2 begin, Vector2 end, Rectangle newSourceRec, const Texture2D* newTexture)
		:
		m_TileGrid(&TileVec),
		m_Begin(begin),
		m_End(end),
		m_NewTexture(newTexture),
		m_NewSourceRec(newSourceRec)
	{
		m_PaintTileVec.reserve(static_cast<size_t>(std::abs(begin.y - end.y) + 1) * static_cast<size_t>(std::abs(begin.x - end.x) + 1));

		for (auto yPos = static_cast<int>(m_Begin.y); yPos < static_cast<int>(m_End.y) + 1; yPos++)
		{
			for (auto xPos = static_cast<int>(m_Begin.x); xPos < static_cast<int>(m_End.x) + 1; xPos++)
			{
				if (!(xPos < 0 || xPos >= m_TileGrid->cols() || yPos < 0 || yPos >= m_TileGrid->rows()))
				{

					Tile* tile = m_TileGrid->at(xPos, yPos);
					m_PaintTileVec.emplace_back(std::make_unique<PaintTileCmd>(*tile, m_NewSourceRec, m_NewTexture));
				}
			}
		}
	}

	void execute() override
	{
		for (auto& tile : m_PaintTileVec)
		{
			tile->execute();
		}
	}

	void undo() override
	{
		for (auto& tile : m_PaintTileVec)
		{
			tile->undo();
		}
	}

private:


	TileGrid* m_TileGrid;

	Vector2 m_Begin = { 0.0f,0.0f };
	Vector2 m_End = { 0.0f,0.0f };

	const Texture2D* m_NewTexture = nullptr;
	Rectangle m_NewSourceRec = { 0.0f, 0.0f, 0.0f, 0.0f };

	std::vector<std::unique_ptr<PaintTileCmd>> m_PaintTileVec;

};


class FloodFill : public Command
{
public:
	FloodFill(TileGrid& TileVec,int xPos,int yPos, Tile* NewTile)
		:
		m_TileGrid(&TileVec),
		m_Xpos(xPos),
		m_Ypos(yPos),
		m_NewTile(NewTile)
	{
		Tile* oldTile = m_TileGrid->at(m_Xpos, m_Ypos);

		if (oldTile == nullptr)
			return;

		Rectangle originalRec = oldTile->scRec;
		unsigned int originalTexId = oldTile->texture->id;

		if ((originalRec.x == m_NewTile->scRec.x) && (originalRec.y == m_NewTile->scRec.y) && (m_NewTile->texture->id == originalTexId))
			return;

		std::queue<Vector2I> positionsQueue;
		positionsQueue.push({ m_Xpos, m_Ypos });

		while (not positionsQueue.empty())
		{
			int newX = positionsQueue.front().x;
			int newY = positionsQueue.front().y;
			positionsQueue.pop();

			Tile* currentTile = m_TileGrid->at(newX, newY);

			if (currentTile == nullptr)
			{
				continue;
			}
			else if (!(currentTile->scRec.x == originalRec.x && currentTile->scRec.y == originalRec.y && currentTile->texture->id == originalTexId))
			{
				continue;
			}
			else
			{
				m_PaintTileVec.emplace_back(std::make_unique<PaintTileCmd>(*currentTile, m_NewTile->scRec, m_NewTile->texture));
				m_PaintTileVec.back()->execute();

				positionsQueue.push({ newX + 1, newY });
				positionsQueue.push({ newX - 1, newY });
				positionsQueue.push({ newX, newY + 1 });
				positionsQueue.push({ newX, newY - 1 });
			}
		}
		m_IsExecuted = true;
	}


private:


	void execute() override
	{
		if (m_IsExecuted != true)
		{
			for (auto& tile : m_PaintTileVec)
			{
				tile->execute();
			}
		}
	}

	void undo() override
	{
		for (auto& tile : m_PaintTileVec)
		{
			tile->undo();
		}
		m_IsExecuted = false;
	}



private:

	

	TileGrid* m_TileGrid;

	int m_Xpos = 0;
	int m_Ypos = 0;

	Tile* m_NewTile = nullptr;

	std::vector<std::unique_ptr<PaintTileCmd>> m_PaintTileVec;

	bool m_IsExecuted = false;

	struct Vector2I
	{
		int x = 0;
		int y = 0;
	};


};


class CommandManager
{
public:
	void execute(std::unique_ptr<Command> cmd)
	{
		//Checks to see if the tile texture is not the same that it will be aplied, if it is it will not be applied
		//if ( ! ((tile.scRec.x == newSourceRec.x) && (tile.scRec.y == newSourceRec.y) && (newTexture == tile.texture)))
		cmd->execute();
		m_UndoStack.push(std::move(cmd));

	}
	void undo()
	{
		if (!m_UndoStack.empty())
		{
			m_UndoStack.top()->undo();
			m_RedoStack.push(std::move(m_UndoStack.top()));
			m_UndoStack.pop();
		}
	}
	void redo()
	{
		if (!m_RedoStack.empty())
		{
			m_RedoStack.top()->execute();
			m_UndoStack.push(std::move(m_RedoStack.top()));
			m_RedoStack.pop();
		}
	}

private:
	std::stack<std::unique_ptr<Command>> m_UndoStack;
	std::stack<std::unique_ptr<Command>> m_RedoStack;
};
