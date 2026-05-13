#pragma once

#include <iostream>

#include <raylib.h>

#include "TileGrid.hpp"
#include "Config.hpp"

struct BackGround
{
    Color color;
    Rectangle body;
};

class TexturePalette
{
public:
    TexturePalette(const Texture2D* texture)
        :
        m_Texture(texture)
    {

    }

    void draw() const
    {
        DrawRectangleRec(m_PaletteBackGround.body, m_PaletteBackGround.color);
        if(m_Texture)
        {
            DrawTextureEx(*m_Texture, m_AnchorPoint, 0.0f, Config::paletteTextureScale, WHITE);
        }
    }


    auto& getTileArr()
    {
        return m_ClickCells.getArr();
    }


    TileGrid& getGrid()
    {
        return m_ClickCells;
    }

    void setTexture(const Texture2D* texture)
    {
        m_Texture = texture;
    }

private:

    const Texture2D* m_Texture = nullptr;

    Vector2 m_AnchorPoint = { 0.0f , 500.0f };
    Vector2 m_TextureSize = { 0, 0 };

    BackGround m_PaletteBackGround = { LIGHTGRAY, { m_AnchorPoint.x, m_AnchorPoint.y, 1400, 500} };

    TileGrid m_ClickCells{ 20, 10, Config::paletteTilesSize ,m_AnchorPoint};


};