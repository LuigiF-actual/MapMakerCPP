#pragma once

#include <raylib.h>
#include <raymath.h>

namespace utilz {
	void movePaletteCam(Camera2D& paletteCamera)
	{
        static bool isDragging = false;
        static Vector2 dragStartMouse = { 0.0f, 0.0f };      // ← Where mouse was when drag started
        static Vector2 dragStartTarget = { 0.0f, 0.0f };     // ← Where camera was when drag started

        if (IsKeyPressed(KEY_X))
        {
            paletteCamera.zoom = 1.0f;
            paletteCamera.target = { 0.0f, 0.0f };
            paletteCamera.offset = { 0.0f, 0.0f };
        }

        // Start dragging
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
            (GetMousePosition().y > ((float)GetScreenHeight() * 70.0f / 100.0f)))
        {
            isDragging = true;
            dragStartMouse = GetMousePosition();
            dragStartTarget = paletteCamera.target;
        }

        // Continue dragging
        if (isDragging && IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 mouseDelta = {
                GetMousePosition().x - dragStartMouse.x,
                GetMousePosition().y - dragStartMouse.y
            };

            paletteCamera.target.x = dragStartTarget.x - mouseDelta.x;  // ← Subtract: drag right = view left
            paletteCamera.target.y = dragStartTarget.y - mouseDelta.y;
        }

        // Stop dragging
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        {
            isDragging = false;
        }
	}
}