#pragma once

#include <iostream>

#include <raylib.h>

#include "TileGrid.hpp"
#include "Config.hpp"
#include "FileExplorer.hpp"
#include "AtlasDictionary.hpp"

struct BackGround
{
    Color color;
    Rectangle body;
};



class TexturePalette
{
public:
    TexturePalette(const Texture2D* texture,Camera2D& camera)
        :
        m_Texture(texture),
        m_PaletteCam(camera)
    {
        m_ClickCells.resetScRecs();
    }


    void update()
    {
        
        if(m_Texture)
        {
            DrawTexture(*m_Texture, m_AnchorPoint.x, m_AnchorPoint.y, WHITE);
        }
        if (IsKeyPressed(KEY_K))
        {
            std::string tempPath = fileExplorer.openExplorer();
            if (!tempPath.empty())
            {
                tempPath = std::filesystem::path(tempPath).stem().string();
                setTexture(&AtlasManager::getInstance().getTexture(tempPath));
            }
        }

        if (IsWindowResized())
        {

            m_AnchorPoint.y = GetScreenHeight() - (GetScreenHeight() * (30.0f / 100.0f));
            if (m_AnchorPoint.y != m_PaletteBackGround.body.y)
            {
                m_ClickCells.setGidPos(m_AnchorPoint);
            }
            m_PaletteBackGround.body.y = m_AnchorPoint.y;
            m_PaletteBackGround.body.width = GetScreenWidth();

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

    Tile* getSelectedCell()
    {
        Tile* newSelect = m_ClickCells.findTile(GetScreenToWorld2D(GetMousePosition(), m_PaletteCam));

        if (newSelect)
        {
             
            //Checks to see if the seletected tile is on the texture
            if ((newSelect->scRec.x > m_Texture->width - Config::paletteTilesSize) || (newSelect->scRec.y > m_Texture->height - Config::paletteTilesSize))
            {

                std::cout << "Calling break\n";
                return nullptr;
            }

        
            if (!m_selectedCell) 
            {
                m_selectedCell = newSelect;
                m_selectedCell->borderColor = ORANGE;
            }

            //If the tile color is not orange that means it was not selected thus I change that here   
            //There can be only ONE orange bordered tile    
            if (!(m_selectedCell == newSelect))
            {
                m_selectedCell->borderColor = BLACK;
                m_selectedCell = newSelect;
                m_selectedCell->borderColor = ORANGE;
            }
        }
        return m_selectedCell;
    }

    void setTexture(const Texture2D* texture)
    {
        std::cout << "Texture Set!" << "\n";
        m_Texture = texture;
        std::cout << "GridSize" << texture->width / 16 << " : : " << texture->height / 16<< "\n";

        m_TextureSize = { (float)m_Texture->width, (float)m_Texture->height };
        std::cout << "New texture size" << m_TextureSize.x << " : : " << m_TextureSize.y << "\n";
    }

    const Texture2D& getTexture()
    {
        return *m_Texture;
    }

    Vector2 getPosition()
    {
        return m_AnchorPoint;
    }

    Vector2 getTextureSize()
    {
        return { m_TextureSize };
    }

    BackGround getBackground()
    {
        return m_PaletteBackGround;
    }

private:

    const Texture2D* m_Texture = nullptr;
    Camera2D& m_PaletteCam;
    Tile* m_selectedCell = nullptr;

    Vector2 m_AnchorPoint = { 0.0f , 300.0f };
    Vector2 m_TextureSize = { 0, 0 };

    BackGround m_PaletteBackGround = { LIGHTGRAY, { m_AnchorPoint.x, m_AnchorPoint.y, Config::screenWidth, Config::screenHeight-m_AnchorPoint.y} };

    TileGrid m_ClickCells{ 20, 10, Config::paletteTilesSize ,m_AnchorPoint};

    FileExplorer fileExplorer;

};