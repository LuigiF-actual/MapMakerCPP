
#include "AppLayer.h"

AppLayer::AppLayer()
{
	std::println("Created new AppLayer!");
}

void AppLayer::OnEvent(Core::Event& event)
{

	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>
	(
	// 	Pass a lambda func to be avaliated by the dispatcher
		[this](Core::MouseButtonPressedEvent& e) 
		{ 
			return OnMouseButtonPressed(e); 
		}
	);

	dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& e) { return OnMouseMoved(e); });
	dispatcher.Dispatch<Core::WindowClosedEvent>([this](Core::WindowClosedEvent& e) { return OnWindowClosed(e); });
	dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) { return OnKeyPressed(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e);  });
	
}

void AppLayer::OnUpdate()
{
	if (m_World == nullptr) { return; }

	m_World->GetBrush().update();
    m_PaletteCamMover.Update();
    m_WorldCamMover.Update();
}

void AppLayer::OnRender()
{
	if (m_World == nullptr) { return; }


    BeginScissorMode(0, 0, GetScreenWidth(), static_cast<int>(m_Palette.getPosition().y));

    BeginMode2D(m_Cam);

    TileRenderer::draw(m_World->GetGrid(), m_Cam);

    EndMode2D();

    EndScissorMode();


	BeginScissorMode(static_cast<int>(m_Palette.getPosition().x), static_cast<int>(m_Palette.getPosition().y), GetScreenWidth(), static_cast<int>(GetScreenHeight() - m_Palette.getPosition().y));

	BeginMode2D(m_PaletteCam);

	m_Palette.update();

	TileRenderer::drawRecLines(m_Palette.getGrid());

	EndMode2D();

	EndScissorMode();

    m_World->GetNavBar().update();
}

bool AppLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event) //NOLINT
{
	if (event.GetMouseButton() == MOUSE_BUTTON_LEFT)
	{
		return true;
	}
	return false;
}

bool AppLayer::OnMouseMoved(Core::MouseMovedEvent& event)
{
	m_MousePosition = { event.GetX(), event.GetY() };
	return false;
}

bool AppLayer::OnWindowClosed(Core::WindowClosedEvent& event)
{
	std::println("Window Closed!");

	return false;
}
//														  clang-tidy was bugging me about this 
bool AppLayer::OnKeyPressed(Core::KeyPressedEvent& event) // NOLINT
{
	if (event.IsRepeat() == false)
	{
        if (event.GetKeyCode() == KEY_T)
        {
            Core::Warnings::messageBox("Fatal", "A texture was not found, make sure all your textures are in the texture folder", "ok", "error", 0);
        }

		if (event.GetKeyCode() == KEY_ESCAPE)
		{
			std::println("App requested switch back to Menu!");
			Core::Application::Get().SwitchLayer<MenuLayer>();
		}
	}
	if (event.IsRepeat())
	{
		if (event.GetKeyCode() == KEY_J)
		{
			std::println("Key {} was repeat once.", event.GetKeyCode());
		}
	}


	return true;
}

bool AppLayer::OnWindowResize(Core::WindowResizeEvent& event)
{
	std::print("AppLayer Heard it!");
    m_Palette.Resize();
	return false;
}

/*
void movePaletteCam(Camera2D& m_PaletteCam)
    {
        static bool m_IsDragging = false;
        static Vector2 m_DragStartMouse = { 0.0f, 0.0f };
        static Vector2 m_DragStartTarget = { 0.0f, 0.0f };

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
            Vector2 m_MouseDelta = {
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

    void moveWorldCam(Camera2D& m_WorldCam)
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
            m_WorldCam.target = {0.0f,0.0f};
            m_WorldCam.offset = {0.0f,0.0f};
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



        utilz::movePaletteCam(paletteCa
 */