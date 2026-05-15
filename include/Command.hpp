#pragma once

#include <stack>
#include <memory>

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

class CommandManager
{
public:
	void execute(Tile& tile,Rectangle newSourceRec ,const Texture2D* newTexture)
	{
		//Checks to see if the tile texture is not the same that it will be aplied, if it is it will not be applied
		if ( ! ((tile.scRec.x == newSourceRec.x) && (tile.scRec.y == newSourceRec.y) && (newTexture == tile.texture)))
		{
			std::cout << "New command created\n";
			std::unique_ptr<PaintTileCmd> paintCommand = std::make_unique<PaintTileCmd>(tile, newSourceRec , newTexture);

			paintCommand->execute();

			m_UndoStack.push(std::move(paintCommand));

		}
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