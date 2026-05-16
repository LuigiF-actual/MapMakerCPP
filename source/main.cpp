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


void moveCamera(Camera2D& camera, Vector2 lockIn);

int main(void)
{

    InitWindow(Config::screenWidth, Config::screenHeight , "MapMakerC++");

    SetWindowState(FLAG_WINDOW_RESIZABLE);

    const Texture2D* mytex = &AtlasManager::getInstance().getFirstTexture();

    Camera2D worldCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    
    
    Camera2D paletteCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
    paletteCam.zoom = 1.0f;

    TexturePalette palette(mytex,paletteCam);

    Rectangle paletteCamBounds = { palette.getPosition().x, palette.getPosition().y, GetScreenWidth(), GetScreenHeight()};

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

        DrawRectangleRec(palette.getBackground().body, palette.getBackground().color);

        BeginScissorMode((int)palette.getPosition().x, (int)palette.getPosition().y, (int)GetScreenWidth(), (int)GetScreenHeight() - (int)palette.getPosition().y);


        //Draws palette tiles
        BeginMode2D(paletteCam);

        palette.update();

        renderer.drawRecLines(palette.getGrid());

        EndMode2D();

        EndScissorMode();

        paintBrush.update();
        
        DrawText(std::to_string(GetFPS()).c_str(), 0.0f, 0.0f, 35, PINK);

        moveCamera(paletteCam,palette.getPosition());

        EndDrawing();
    }

    CloseWindow();       

    return 0;
}

void moveCamera(Camera2D& camera,Vector2 lockIn)
{
    if (IsKeyDown(KEY_W))
    {
        camera.target.y -= 250.0f * GetFrameTime();
    }
    if (IsKeyDown(KEY_S))
    {
        camera.target.y += 250.0f * GetFrameTime();
    }
    if (IsKeyDown(KEY_D))
    {
        camera.target.x += 250.0f * GetFrameTime();
    }
    if (IsKeyDown(KEY_A))
    {
        camera.target.x -= 250.0f * GetFrameTime();
    }
    if (IsKeyPressed(KEY_E))
    {
        camera.zoom += 0.2f;
        std::cout << camera.zoom << "\n";
    }
    if (IsKeyPressed(KEY_Q))
    {
        camera.zoom -= 0.2f;
        std::cout << camera.zoom << "\n";
    }
    if (IsKeyPressed(KEY_X))
    {
        camera.zoom = 1.0f;
        camera.target = {0.0f,0.0f};
        camera.offset = {0.0f,0.0f};
        std::cout << "locked in" << "\n ";
    }
}
