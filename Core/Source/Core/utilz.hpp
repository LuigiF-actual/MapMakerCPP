#pragma once

#include <raylib.h>
#include <raymath.h>
#include <Config.hpp>


namespace utilz {




	void movePaletteCam(Camera2D& paletteCamera)
	{
        static bool isDragging = false;
        static Vector2 dragStartMouse = { 0.0f, 0.0f };     
        static Vector2 dragStartTarget = { 0.0f, 0.0f };    

        if (IsKeyPressed(KEY_X))
        {
            paletteCamera.zoom = 1.0f;
            paletteCamera.target = { 0.0f, 0.0f };
            paletteCamera.offset = { 0.0f, 0.0f };
        }

        // Start the drag
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
            (GetMousePosition().y > (static_cast<float>(GetScreenHeight()) * 70.0f / 100.0)))
        {
            isDragging = true;
            dragStartMouse = GetMousePosition();
            dragStartTarget = paletteCamera.target;
        }

        // Continue the drag
        if (isDragging && IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mouseDelta = {
                GetMousePosition().x - dragStartMouse.x,
                GetMousePosition().y - dragStartMouse.y
            };

            paletteCamera.target.x = dragStartTarget.x - mouseDelta.x; 
            paletteCamera.target.y = dragStartTarget.y - mouseDelta.y;
        }

        // Stop the drag
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        {
            isDragging = false;
        }

        if (GetMouseWheelMove() != 0)
        {
            paletteCamera.zoom = expf(logf(paletteCamera.zoom) + (GetMouseWheelMove() * Config::zoomSpd));
        }
	}
    
    void moveWorldCam(Camera2D& worldCamera)
    {
        if (IsKeyDown(KEY_W))
        {
            worldCamera.target.y -= Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyDown(KEY_S))
        {
            worldCamera.target.y += Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyDown(KEY_D))
        {
            worldCamera.target.x += Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyDown(KEY_A))
        {
            worldCamera.target.x -= Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyPressed(KEY_E))
        {
            worldCamera.zoom = expf(logf(worldCamera.zoom) + Config::zoomSpd);
            std::cout << worldCamera.zoom << "\n";
        }
        if (IsKeyPressed(KEY_Q))
        {
            worldCamera.zoom = expf(logf(worldCamera.zoom) - Config::zoomSpd);
            std::cout << worldCamera.zoom << "\n";
        }
        if (IsKeyPressed(KEY_X))
        {
            worldCamera.zoom = 1.0f;
            worldCamera.target = {0.0f,0.0f};
            worldCamera.offset = {0.0f,0.0f};
            std::cout << "locked in" << "\n ";
        }
    }

    void drawEditor(TileGrid& worldGrid, TexturePalette& palette,TileRenderer& renderer,PaintBrush& paintBrush, Camera2D& worldCam, Camera2D& paletteCam)
    {
        BeginScissorMode(0, 0, GetScreenWidth(), static_cast<int>(palette.getPosition().y));

        BeginMode2D(worldCam);

        renderer.draw(worldGrid, worldCam);

        EndMode2D();

        EndScissorMode();

        utilz::moveWorldCam(worldCam);


        //background of the pallete
        DrawRectangleRec(palette.getBackground().body, palette.getBackground().color);

        //Palette part of the screen
        BeginScissorMode(static_cast<int>(palette.getPosition().x), static_cast<int>(palette.getPosition().y), GetScreenWidth(), static_cast<int>(GetScreenHeight() - palette.getPosition().y));

        BeginMode2D(paletteCam);

        palette.update();

        renderer.drawRecLines(palette.getGrid());

        EndMode2D();

        EndScissorMode();

        paintBrush.update();



        utilz::movePaletteCam(paletteCam);
    }

}