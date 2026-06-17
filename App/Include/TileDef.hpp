#pragma once 

struct Tile
{
	const Texture2D* texture;
	Rectangle body;
	Rectangle scRec;
	Color borderColor = BLACK;
};