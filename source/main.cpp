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
#include <DataBase.hpp>
#include <Menu.hpp>
#include <SavesFileManager.hpp>

enum class Scene : unsigned char
{
    MENU,
    EDITOR
};

static int* nana()
{
    return nullptr;
}

int main(void)
{
    InitWindow(Config::screenWidth, Config::screenHeight , "MapMakerC++");

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();
    SetExitKey(KEY_NULL);
    Scene scene = Scene::EDITOR;


    
    Camera2D paletteCam = { {0.0F,0.0F},{0.0F,0.0F},0.0F,1.0F };
    TexturePalette palette(&AtlasManager::getInstance().getTexture(Config::default_Atlas), paletteCam);


    Menu menu;

    Camera2D worldCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    TileGrid worldGrid(10, 10, 64.0f, { -100.0f, -100.0f });
    TileRenderer renderer;

    PaintBrush paintBrush(worldGrid,palette,worldCam);

    SavesFilesManager saveFiles(menu);
    saveFiles.createSave("superTest");


    if (nana() != nullptr) {
        std::cout << "lol\n";
    }

    while (!WindowShouldClose())   
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        
        //World part of the screen

        if (IsKeyPressed(KEY_ESCAPE) && (scene == Scene::EDITOR))
        {
            scene = Scene::MENU;
            menu.setMenu(true);
            menu.setMenuState(MenuAction::NONE);
        } else if (IsKeyPressed(KEY_ESCAPE) && (scene == Scene::MENU))
        {
            scene = Scene::EDITOR;
        }

        switch (scene)
        {
        case Scene::EDITOR:
            utilz::drawEditor(worldGrid,palette,renderer,paintBrush,worldCam,paletteCam);
            break;

        case Scene::MENU:
            menu.update();
            break;
        }

        DrawText(std::to_string(GetFPS()).c_str(), 0.0f, 0.0f, 35, GREEN);


        EndDrawing();
    }

    CloseWindow();       

    return 0;
}

