#pragma once

#include <vector>
#include <print>

#include "Core/SaveData.hpp"

#include <raylib.h>

#include "TileGrid.hpp"
#include "PaintBrush.hpp"
#include "PaletteGrid.hpp"
#include "NavBar.hpp"



class World
{
public:
    World(int gridW, int gridH, std::vector<Core::TileSaveData>& vecToLoad, TexturePalette& palette, Camera2D& cam)
        :
        m_WorldGrid{ gridW,gridH,Config::tileSize,vecToLoad},
        m_PaintBrush{ m_WorldGrid ,palette,cam },
        m_NavBar{m_PaintBrush.GetPmode(),m_PaintBrush.GetRedoBool(),m_PaintBrush.GetUndoBool()}
    {

    }

    World(int gridW, int gridH, TexturePalette& palette, Camera2D& cam)
        :
        m_WorldGrid{ gridW,gridH,Config::tileSize,Vector2{0.0f,0.0f} },
        m_PaintBrush{ m_WorldGrid ,palette,cam },
        m_NavBar{ m_PaintBrush.GetPmode(),m_PaintBrush.GetRedoBool(),m_PaintBrush.GetUndoBool() }
    {
    }

    TileGrid& GetGrid() { return m_WorldGrid; }
    PaintBrush& GetBrush() { return m_PaintBrush; }
    NavBar& GetNavBar() { return m_NavBar; }

private:
    TileGrid m_WorldGrid;
    PaintBrush m_PaintBrush;
    NavBar m_NavBar;
};