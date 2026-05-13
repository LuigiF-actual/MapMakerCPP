#pragma once

#include <raylib.h>


class KeyboardManager
{
public:
	bool ctrlZ()
	{
		if (IsKeyDown(KEY_LEFT_CONTROL))
		{
			if (IsKeyPressed(KEY_Z))
				return true;
		}
		return false;
	}

	bool ctrlY()
	{
		if (IsKeyDown(KEY_LEFT_CONTROL))
		{
			if (IsKeyPressed(KEY_Y))
				return true;
		}
		return false;
	}

};
