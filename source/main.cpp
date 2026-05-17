// MapMakerC++.cpp : Defines the entry point for the application.

#include <iostream>

#include <raylib.h>
#include <AtlasDictionary.hpp>
#include <TileGrid.hpp>
#include <PaintBrush.hpp>
#include <TexturePalette.hpp>
#include <Config.hpp>
#include <FileExplorer.hpp>
#include <string>
#include <utilz.hpp>


void moveCamera(Camera2D& camera, Vector2 lockIn);
void moveCamera2(Camera2D& camera, Vector2 lockIn);

int main(void)
{
    InitWindow(Config::screenWidth, Config::screenHeight , "MapMakerC++");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();

    
    
    Camera2D paletteCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    TexturePalette palette(&AtlasManager::getInstance().getFirstTexture(),paletteCam);
    Rectangle paletteCamBounds = { palette.getPosition().x, palette.getPosition().y, GetScreenWidth(), GetScreenHeight()};



    Camera2D worldCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    TileGrid worldGrid(100, 100, 64.0f, { -100.0f, -100.0f });
    TileRenderer renderer;


    PaintBrush paintBrush(worldGrid, palette,paletteCam);


    while (!WindowShouldClose())   
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        //Draws map tiles
        BeginMode2D(worldCam);

        renderer.draw(worldGrid,worldCam);

        EndMode2D();
        moveCamera2(worldCam,{0,0});

        DrawRectangleRec(palette.getBackground().body, palette.getBackground().color);

        BeginScissorMode((int)palette.getPosition().x, (int)palette.getPosition().y, (int)GetScreenWidth(), (int)GetScreenHeight() - (int)palette.getPosition().y);


        //Draws palette tiles
        BeginMode2D(paletteCam);

        palette.update();

        renderer.drawRecLines(palette.getGrid());

        EndMode2D();

        utilz::moveWorldCam(worldCam);

        EndScissorMode();

        paintBrush.update();
        
        DrawText(std::to_string(GetFPS()).c_str(), 0.0f, 0.0f, 35, PINK);

        utilz::movePaletteCam(paletteCam);

        EndDrawing();
    }

    CloseWindow();       

    return 0;
}

