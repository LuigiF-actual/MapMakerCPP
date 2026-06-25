
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

	return false;
}
//														  clang-tidy was bugging me about this 
bool AppLayer::OnKeyPressed(Core::KeyPressedEvent& event) // NOLINT
{
	if (event.IsRepeat() == false)
	{

		if (event.GetKeyCode() == KEY_ESCAPE)
		{
			Core::Application::Get().SwitchLayer<MenuLayer>();
		}
	}


	return true;
}

bool AppLayer::OnWindowResize(Core::WindowResizeEvent& event)
{
    m_Palette.Resize();
	return false;
}

