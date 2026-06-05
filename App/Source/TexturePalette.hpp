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
        
        if(m_Texture != nullptr)
        {
            DrawTexture(*m_Texture, static_cast<int>(m_AnchorPoint.x), static_cast<int>(m_AnchorPoint.y), WHITE);
        }
        if (IsKeyPressed(KEY_K))
        {
            std::string tempPath = m_FileExplorer.openExplorer();
            if (!tempPath.empty())
            {
                tempPath = std::filesystem::path(tempPath).stem().string();
                setTexture(&AtlasManager::getInstance().getTexture(tempPath));
            }
        }

        if (IsWindowResized())
        {

            m_AnchorPoint.y = static_cast<float>(GetScreenHeight()) - (GetScreenHeight() * (30.0f / 100.0f));
            if (m_AnchorPoint.y != m_PaletteBackGround.body.y)
            {
                m_ClickCells.setGidPos(m_AnchorPoint);
            }
            m_PaletteBackGround.body.y = m_AnchorPoint.y;
            m_PaletteBackGround.body.width = static_cast<float>(GetScreenWidth());

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

        if (newSelect != nullptr)
        {
             
            //Checks to see if the seletected tile is on the texture
            if ((newSelect->scRec.x > m_Texture->width - Config::paletteTilesSize) || (newSelect->scRec.y > m_Texture->height - Config::paletteTilesSize))
            {

                return nullptr;
            }

        
            //m_SelectedCell always is assigned nullptr upon class initialization
            //This if is for when the player first clicks the palette grid
            if (m_SelectedCell == nullptr) 
            {
                m_SelectedCell = newSelect;
                m_SelectedCell->texture = m_Texture;
                m_SelectedCell->borderColor = ORANGE;
            }


            //Avoid useless asigns if the user clicks the same 
            //cell twice the operation will not be performed
            if ( m_SelectedCell != newSelect )
            {
                m_SelectedCell->borderColor = BLACK;
                m_SelectedCell = newSelect;
                m_SelectedCell->borderColor = ORANGE;
            }
        }
        return m_SelectedCell;
    }

    void setTexture(const Texture2D* texture)
    {
        m_Texture = texture;
        m_TextureSize = { static_cast<float>(m_Texture->width), static_cast<float>(m_Texture->height) };
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
    Tile* m_SelectedCell = nullptr;

    Vector2 m_AnchorPoint = { 0.0f , 300.0f };
    Vector2 m_TextureSize = { 0, 0 };

    BackGround m_PaletteBackGround = { LIGHTGRAY, { m_AnchorPoint.x, m_AnchorPoint.y, Config::screenWidth, Config::screenHeight-m_AnchorPoint.y} };

    TileGrid m_ClickCells{ 20, 10, Config::paletteTilesSize ,m_AnchorPoint};

    FileExplorer m_FileExplorer;

};