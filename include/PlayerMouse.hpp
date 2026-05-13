#pragma once

#include <raylib.h>


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

private:

};
