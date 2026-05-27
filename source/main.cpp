// MapMakerC++.cpp : Defines the entry point for the application.

#include <iostream>

#include <raylib.h>
#include <sqlite3.h>
#include <tinyfiledialogs.h>


#include <AtlasDictionary.hpp>
#include <TileGrid.hpp>
#include <PaintBrush.hpp>
#include <TexturePalette.hpp>
#include <Config.hpp>
#include <FileExplorer.hpp>
#include <string>
#include <utilz.hpp>
#include <HelpScreen.hpp>
#include <DataBase.hpp>
#include <Menu.hpp>

enum class Scene : unsigned char
{
    MENU,
    EDITOR
};

int main(void)
{
    InitWindow(Config::screenWidth, Config::screenHeight , "MapMakerC++");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();

    Scene scene = Scene::EDITOR;

    DataBase DB;
    DB.createTable();

    
    Camera2D paletteCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    TexturePalette palette(&AtlasManager::getInstance().getTexture(Config::default_Atlas.data()), paletteCam);
    Rectangle paletteCamBounds = { palette.getPosition().x, palette.getPosition().y, (float)GetScreenWidth(), (float)GetScreenHeight()};

    Menu menu;

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

        if (IsKeyPressed(KEY_R))
        {
            scene = Scene::MENU;
        }
        if (IsKeyPressed(KEY_T))
        {
            scene = Scene::EDITOR;
        }

        switch (scene)
        {
        case Scene::EDITOR:
            utilz::drawEditor(worldGrid,palette,renderer,paintBrush,worldCam,paletteCam);
            break;

        case Scene::MENU:
            menu.draw();
            break;
        }


        EndDrawing();
    }

    CloseWindow();       

    return 0;
}

