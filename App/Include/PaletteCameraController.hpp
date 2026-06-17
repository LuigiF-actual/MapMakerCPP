#pragma once

#include <raylib.h>
#include <raymath.h>
#include "Config.hpp"

class PaletteCameraController
{
public:
	PaletteCameraController(Camera2D& cam)
		:
		m_PaletteCam(cam)
	{}

	~PaletteCameraController() = default;


    void Update()
    {
        if (IsKeyPressed(KEY_X))
        {
            m_PaletteCam.zoom = 1.0f;
            m_PaletteCam.target = { 0.0f, 0.0f };
            m_PaletteCam.offset = { 0.0f, 0.0f };
        }

        // Start the drag
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) &&
            (GetMousePosition().y > (static_cast<float>(GetScreenHeight()) * 70.0f / 100.0)))
        {
            m_IsDragging = true;
            m_DragStartMouse = GetMousePosition();
            m_DragStartTarget = m_PaletteCam.target;
        }

        // Continue the drag
        if (m_IsDragging && IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            m_MouseDelta = {
                GetMousePosition().x - m_DragStartMouse.x,
                GetMousePosition().y - m_DragStartMouse.y
            };

            m_PaletteCam.target.x = m_DragStartTarget.x - m_MouseDelta.x;
            m_PaletteCam.target.y = m_DragStartTarget.y - m_MouseDelta.y;
        }

        // Stop the drag
        if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT))
        {
            m_IsDragging = false;
        }

        if (GetMouseWheelMove() != 0)
        {
            m_PaletteCam.zoom = expf(logf(m_PaletteCam.zoom) + (GetMouseWheelMove() * Config::zoomSpd));
        }
    }

private:

	bool m_IsDragging = false;

    Vector2 m_MouseDelta = { 0.0f,0.0f };
	Vector2 m_DragStartMouse = { 0.0f, 0.0f };
	Vector2 m_DragStartTarget = { 0.0f, 0.0f };
	
	Camera2D& m_PaletteCam;        
};
