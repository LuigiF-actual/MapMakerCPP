#pragma once

#include <raylib.h>
#include <raymath.h>

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
            (GetMousePosition().y > ((float)GetScreenHeight() * 70.0f / 100.0f)))
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
            paletteCamera.zoom = expf(logf(paletteCamera.zoom) + ((float)GetMouseWheelMove() * 0.1f));
        }
	}
    
    void moveWorldCam(Camera2D& worldCamera)
    {
        if (IsKeyDown(KEY_W))
        {
            worldCamera.target.y -= 250.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_S))
        {
            worldCamera.target.y += 250.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_D))
        {
            worldCamera.target.x += 250.0f * GetFrameTime();
        }
        if (IsKeyDown(KEY_A))
        {
            worldCamera.target.x -= 250.0f * GetFrameTime();
        }
        if (IsKeyPressed(KEY_E))
        {
            worldCamera.zoom += 100.0f * GetFrameTime();
            std::cout << worldCamera.zoom << "\n";
        }
        if (IsKeyPressed(KEY_Q))
        {
            worldCamera.zoom -= 0.2f;
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
}