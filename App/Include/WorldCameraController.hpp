#pragma once

#include <raylib.h>
#include <raymath.h>
#include "Config.hpp"


class WorldCameraController
{
public:
    WorldCameraController(Camera2D& worldCam)
        :
        m_WorldCam(worldCam)
	{}

	~WorldCameraController() = default;


    void Update()
    {
        if (IsKeyDown(KEY_W))
        {
            m_WorldCam.target.y -= Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyDown(KEY_S))
        {
            m_WorldCam.target.y += Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyDown(KEY_D))
        {
            m_WorldCam.target.x += Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyDown(KEY_A))
        {
            m_WorldCam.target.x -= Config::worldCamMoveSpd * GetFrameTime();
        }
        if (IsKeyPressed(KEY_E))
        {
            m_WorldCam.zoom = expf(logf(m_WorldCam.zoom) + Config::zoomSpd);
            std::cout << m_WorldCam.zoom << "\n";
        }
        if (IsKeyPressed(KEY_Q))
        {
            m_WorldCam.zoom = expf(logf(m_WorldCam.zoom) - Config::zoomSpd);
            std::cout << m_WorldCam.zoom << "\n";
        }
        if (IsKeyPressed(KEY_X))
        {
            m_WorldCam.zoom = 1.0f;
            m_WorldCam.target = { 0.0f,0.0f };
            m_WorldCam.offset = { 0.0f,0.0f };
        }
    }

private:

    Camera2D& m_WorldCam;

 };




