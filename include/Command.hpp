#pragma once

#include <stack>
#include <memory>
#include <vector>

#include <raylib.h>
#include "TileGrid.hpp"
#include "AtlasDictionary.hpp"
#include "Config.hpp"


class Command
{
public:
	virtual ~Command() {}
	virtual void execute() = 0;
	virtual void undo() = 0;
};	

class PaintTileCmd : public Command
{
public:
	PaintTileCmd(Tile& tile, Rectangle newSourceRec ,const Texture2D* newTexture)
		: 
		mp_ChangedTile(&tile),
		m_NewTexture(newTexture),
		m_NewSouceRec(newSourceRec),
		m_OriginalSourceRec(tile.scRec),
		m_OriginalTexture(tile.texture)
	{
	}

	void execute() override
	{
		if (mp_ChangedTile)
		{
			mp_ChangedTile->texture = m_NewTexture;
			mp_ChangedTile->scRec = m_NewSouceRec;
		}
	}

	void undo() override
	{
		if (mp_ChangedTile)
		{
			mp_ChangedTile->texture = m_OriginalTexture;
			mp_ChangedTile->scRec = m_OriginalSourceRec;
		}
	}
private:
	const Texture2D* m_OriginalTexture = nullptr;
	const Texture2D* m_NewTexture = nullptr;

	Rectangle m_NewSouceRec = { 0 };
	Rectangle m_OriginalSourceRec = { 0 };

	Tile* mp_ChangedTile = nullptr;
};

class RectangleFillCmd : public Command
{
public:
	RectangleFillCmd(TileGrid& TileVec,Vector2 begin, Vector2 end, Rectangle newSourceRec, const Texture2D* newTexture)
		:
		m_TileGrid(&TileVec),
		m_Begin(begin),
		m_End(end),
		m_NewTexture(newTexture),
		m_NewSourceRec(newSourceRec)
	{
		m_PaintTileVec.reserve(size_t(std::abs(begin.y - end.y)+1) * size_t(std::abs(begin.x - end.x)+1));
		
		//std::cout << "Rec total size" << size_t(std::abs(begin.y - end.y)+1) * size_t(std::abs(begin.x - end.x)+1) << "\n";

		for (auto y = int(m_Begin.y ); y < int(m_End.y ) + 1; y++)
		{
			for (auto x = int(m_Begin.x); x < int(m_End.x ) + 1; x++)
			{
				if (!(x < 0 || x >= m_TileGrid->cols() || y < 0 || y >= m_TileGrid->rows()))
				{

					Tile* tile = m_TileGrid->at(x, y);
					m_PaintTileVec.emplace_back(std::make_unique<PaintTileCmd>(*tile, m_NewSourceRec, m_NewTexture));
				}
			}
		}
	}

	void execute() override
	{
		for (auto& i : m_PaintTileVec)
		{
			i->execute();
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

	Vector2 m_Begin = { 0 };
	Vector2 m_End = { 0 };

	const Texture2D* m_NewTexture = nullptr;
	Rectangle m_NewSourceRec = { 0 };

	std::vector<std::unique_ptr<PaintTileCmd>> m_PaintTileVec;

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