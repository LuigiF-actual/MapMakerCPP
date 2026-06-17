#pragma once

#include <vector>
#include <print>

#include "Core/SaveData.hpp"

#include <raylib.h>

#include "TileGrid.hpp"
#include "PaintBrush.hpp"
#include "PaletteGrid.hpp"



class World
{
public:
    World(int gridW, int gridH, std::vector<Core::TileSaveData>& vecToLoad, TexturePalette& palette, Camera2D& cam)
        :
        m_WorldGrid{ gridW,gridH,Config::tileSize,vecToLoad},
        m_PaintBrush{ m_WorldGrid ,palette,cam }
    {

    }

    World(int gridW, int gridH, TexturePalette& palette, Camera2D& cam)
        :
        m_WorldGrid{ gridW,gridH,Config::tileSize,Vector2{0.0f,0.0f} },
        m_PaintBrush{ m_WorldGrid ,palette,cam }
    {
        std::println("NewWorldCreated {} {}", gridW, gridH);
    }

    TileGrid& GetGrid() { return m_WorldGrid; }
    PaintBrush& GetBrush() { return m_PaintBrush; }

private:
    TileGrid m_WorldGrid;
    PaintBrush m_PaintBrush;
};