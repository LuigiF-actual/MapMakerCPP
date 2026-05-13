// MapMakerC++.cpp : Defines the entry point for the application.

#include <iostream>

#include <raylib.h>
#include <AtlasDictionary.hpp>
#include <TileGrid.hpp>
#include <PaintBrush.hpp>
#include <TexturePalette.hpp>

int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "MapMakerC++");

    SetTargetFPS(60);         

    const Texture2D* mytex = &AtlasManager::getInstance().getFirstTexture();
    std::cout << mytex->width << "  _____  " << mytex->height << "\n";

    TexturePalette palette(mytex);
    TileGrid worldGrid(30, 30, 100.0f, { 0.0f, 0.0f });
    TileRenderer renderer;

    while (!WindowShouldClose())   
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        renderer.draw(worldGrid);

        palette.draw();
        renderer.drawRecLines(palette.getGrid());

        EndDrawing();
    }

    CloseWindow();       

    return 0;
}
