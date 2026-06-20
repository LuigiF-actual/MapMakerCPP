#include "PaintBrush.hpp"


void PaintBrush::checkKeyboard()
{

	if(IsKeyPressed(KEY_B))
	{
		m_Mode = PaintMode::NORMAL;
	}

	if (IsKeyPressed(KEY_U))
	{
		m_Mode = PaintMode::RECTANGLE;
	}

	if (IsKeyPressed(KEY_G))
	{
		m_Mode = PaintMode::FILL;
	}

	if (m_Keyboard.ctrlZ() || m_UndoCmd)
	{
		std::print("Undo");
		m_Cmd.undo();
		m_UndoCmd = false;
	}
	if (m_Keyboard.ctrlY() || m_RedoCmd)
	{
		std::print("Redo");
		m_Cmd.redo();
		m_RedoCmd = false;
	}

}

void PaintBrush::checkMouseInput()
{

	if (GetMousePosition().y > GetScreenHeight() * 0.05f)
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
			case PaintMode::FILL:
				bucketFill();
				break;
			}
		}
	}
}

void PaintBrush::rectangleFill()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_Begin = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_WorldCam), m_TileGrid.offset());
	}
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && (m_SelectedCell != nullptr))
	{
		m_End = Vector2Subtract(GetScreenToWorld2D(GetMousePosition(), m_WorldCam), m_TileGrid.offset());


		float beginX = static_cast<float>(std::min(m_Begin.x, m_End.x) / Config::tileSize);
		float beginY = static_cast<float>(std::min(m_Begin.y, m_End.y) / Config::tileSize);

		float endX = static_cast<float>(std::max(m_Begin.x, m_End.x) / Config::tileSize);
		float endY = static_cast<float>(std::max(m_Begin.y, m_End.y) / Config::tileSize);

		auto fillRec = std::make_unique<RectangleFillCmd>(m_TileGrid, Vector2{ beginX,beginY }, Vector2{ endX, endY }, m_SelectedCell->scRec, &m_TexturesPallete.getTexture());
		m_Cmd.execute(std::move(fillRec));

		m_Begin = { 0.0f,0.0f };
		m_End = { 0.0f,0.0f };
	}
}

void PaintBrush::paintTile()
{
	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
	{
		Tile* p_Tile = m_TileGrid.findTile(GetScreenToWorld2D(GetMousePosition(), m_WorldCam));

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

void PaintBrush::pickPaletteCell()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		m_SelectedCell = m_TexturesPallete.getSelectedCell();
	}
}

void PaintBrush::bucketFill()
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
	{
		Tile* p_Tile = m_TileGrid.findTile(GetScreenToWorld2D(GetMousePosition(), m_WorldCam));
		if ((p_Tile != nullptr) && (m_SelectedCell != nullptr))
		{

			if (p_Tile->scRec.x != m_SelectedCell->scRec.x ||
				p_Tile->scRec.y != m_SelectedCell->scRec.y ||
				&m_TexturesPallete.getTexture() != p_Tile->texture)
			{
				auto paintCmd = std::make_unique<FloodFill>(m_TileGrid, static_cast<int>(p_Tile->body.x / Config::tileSize), static_cast<int>(p_Tile->body.y / Config::tileSize), m_SelectedCell);
				m_Cmd.execute(std::move(paintCmd));
				std::print("BucketTool");
			}
		}
	}
	
}
