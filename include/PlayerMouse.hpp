#pragma once

#include <raylib.h>
#include <Config.hpp>

class PlayerMouse
{
public:
	PlayerMouse() = default;
	
	Vector2 getMousePos()
	{
		return GetMousePosition();
	}

	bool isMouseClicked()
	{
		return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
	}

	bool isMousePressed()
	{
		return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
	}

	Vector2 MousePos2AtlasPos(Vector2 atlasPos)
	{
		Vector2 AtlasPos = {
			float(int((getMousePos().x - atlasPos.x) / Config::paletteTextureScale / 16) * 16),
			float(int((getMousePos().y - atlasPos.y) / Config::paletteTextureScale / 16) * 16)
		};

		return AtlasPos;
	}

private:

};
