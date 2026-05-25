// MapMakerC++.cpp : Defines the entry point for the application.

#include <iostream>

#include <raylib.h>
#include <sqlite3.h>


#include <AtlasDictionary.hpp>
#include <TileGrid.hpp>
#include <PaintBrush.hpp>
#include <TexturePalette.hpp>
#include <Config.hpp>
#include <FileExplorer.hpp>
#include <string>
#include <utilz.hpp>
#include <HelpScreen.hpp>

#include <tinyfiledialogs.h>

int main(void)
{
    InitWindow(Config::screenWidth, Config::screenHeight , "MapMakerC++");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();

    
    Camera2D paletteCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    TexturePalette palette(&AtlasManager::getInstance().getTexture(Config::default_Atlas.data()), paletteCam);
    Rectangle paletteCamBounds = { palette.getPosition().x, palette.getPosition().y, GetScreenWidth(), GetScreenHeight()};



    Camera2D worldCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    TileGrid worldGrid(1000, 1000, 64.0f, { -100.0f, -100.0f });
    TileRenderer renderer;


    PaintBrush paintBrush(worldGrid, palette,paletteCam,worldCam);
    HelpScreen help;


    while (!WindowShouldClose())   
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        //World part of the screen

        BeginScissorMode(0.0f, 0.0f, (int)GetScreenWidth(), (int)palette.getPosition().y);

            BeginMode2D(worldCam);

            renderer.draw(worldGrid,worldCam);

            EndMode2D();

        EndScissorMode();
        
        utilz::moveWorldCam(worldCam);


        //background of the pallete
        DrawRectangleRec(palette.getBackground().body, palette.getBackground().color);

        //Palette part of the screen
        BeginScissorMode((int)palette.getPosition().x, (int)palette.getPosition().y, (int)GetScreenWidth(), (int)GetScreenHeight() - (int)palette.getPosition().y);

            BeginMode2D(paletteCam);

            palette.update();

            renderer.drawRecLines(palette.getGrid());

            EndMode2D();

        EndScissorMode();

        paintBrush.update();
        
        DrawText(std::to_string(GetFPS()).c_str(), 0.0f, 0.0f, 35, PINK);

        if (IsKeyDown(KEY_H))
        {
            help.draw();
        }

        utilz::movePaletteCam(paletteCam);
        

        EndDrawing();
    }

    CloseWindow();       

    return 0;
}

