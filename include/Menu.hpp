#pragma once

#include <raylib.h>

class Menu
{
public:
	Menu()
	{}

	~Menu()
	{}

	void draw()
	{
		DrawRectangle(0.0f, 0.0f, GetScreenWidth(), GetScreenHeight(), Color{ 206, 81, 240 , 255 });
	}

private:

};

