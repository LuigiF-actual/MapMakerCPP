#include "MenuLayer.h"


void MenuLayer::OnUpdate()
{
	if (m_Menu.ActionSubmited())
	{
		switch(m_Menu.getMenuState())
		{
		case MenuAction::NONE:
		{
			break;
		}
		case MenuAction::NEW_PROJECT:
		{
			auto* appLayer = Core::Application::Get().GetLayer<AppLayer>();
			appLayer->LoadNewWorld(m_Menu.GetNewProjectConfig().gridWidth, m_Menu.GetNewProjectConfig().gridHeight);

			std::print("NewWord Loaded, switching");
			Core::Application::Get().SwitchLayer<AppLayer>();
			break;
		}
		case MenuAction::OPEN_PROJECT:
		{
			std::println("Trying to load, {} ", m_Menu.GetPathToSave().string());

			auto* appLayer = Core::Application::Get().GetLayer<AppLayer>();
			Core::SaveData saveData = Core::Application::Get().GetSaveManager().LoadSave(m_Menu.GetPathToSave());

			std::println("save data created! {} ",saveData.TileSDataVec.at(0).spriteName);

			appLayer->LoadSavedWorld(saveData.WGridData.gridW, saveData.WGridData.gridH, saveData.TileSDataVec);

			std::print("Saved World Loaded, switching");
			Core::Application::Get().SwitchLayer<AppLayer>();

			break;
		}
		case MenuAction::SAVE_PROJECT:
		{
			auto* appLayer = Core::Application::Get().GetLayer<AppLayer>();
			appLayer->SaveWorld();
			std::cout << "Here\n";
			break;
		}

		}
		m_Menu.ResetMenuState();
	}
}

void MenuLayer::OnRender()
{
	m_Menu.update();
}

void MenuLayer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& e) { return OnKeyPressed(e); });
	dispatcher.Dispatch<Core::WindowResizeEvent>([this](Core::WindowResizeEvent& e) { return OnWindowResize(e); });
}

bool MenuLayer::OnKeyPressed(Core::KeyPressedEvent& event)
{
	if (event.IsRepeat() == false)
	{
		if (event.GetKeyCode() == KEY_ESCAPE)
		{
			Core::Application::Get().SwitchLayer<AppLayer>();
			return true;
		}

	}
	return false;
}

bool MenuLayer::OnWindowResize(Core::WindowResizeEvent& event) // NOLINT
{
	m_Menu.resizeMenu();

	return false;
}
