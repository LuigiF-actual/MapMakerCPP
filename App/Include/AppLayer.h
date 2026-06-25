#pragma once


#include <print>
#include <stdint.h>
#include <memory>

#include "raylib.h"

#include "Core/Layer.h"
#include "Core/InputEvents.h"
#include "Core/WindowEvents.h"
#include "Core/Application.h"
#include "Core/SaveData.hpp"
#include "Core/PopUpWarnings.hpp"

#include "MenuLayer.h"

#include "AtlasManager.hpp"
#include "PaletteGrid.hpp"
#include "PaletteCameraController.hpp"

#include "TileGrid.hpp"
#include "World.hpp"
#include "WorldCameraController.hpp"

class AppLayer : public Core::Layer
{
public:
	AppLayer();
	~AppLayer() override = default;

	void OnEvent(Core::Event& event) override;

	void OnUpdate() override;
	void OnRender() override;

	void LoadSavedWorld(int gridW,int gridH, std::vector<Core::TileSaveData> dataToLoad)
	{
		m_World = std::make_unique<World>(gridW, gridH, dataToLoad, m_Palette, m_Cam);
	}

	void LoadNewWorld(int gridW, int gridH)
	{
		m_World = std::make_unique<World>(gridW, gridH, m_Palette, m_Cam);
	}


	void SaveWorld()
	{
		if (m_World == nullptr) { return; }

		auto tiles = m_World->GetGrid().DataToBeSaved();
		Core::Application::Get().GetSaveManager().SaveGame(tiles);
	}


private:

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);
	bool OnMouseMoved(Core::MouseMovedEvent& event);
	bool OnWindowClosed(Core::WindowClosedEvent& event);
	bool OnKeyPressed(Core::KeyPressedEvent& event);
	bool OnWindowResize(Core::WindowResizeEvent& event);


private:

	Vector2 m_MousePosition{ 0.0f };
	int m_KeyPressed = 0;

	Camera2D m_Cam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };
	Camera2D m_PaletteCam = { {0.0f,0.0f},{0.0f,0.0f},0.0f,1.0f };

	TexturePalette m_Palette{ &AtlasManager::getInstance().getTexture(Config::getDefault_Atlas()), m_PaletteCam };

	std::unique_ptr<World> m_World; 
	WorldCameraController m_WorldCamMover{ m_Cam };

	TileRenderer m_Renderer;

	PaletteCameraController m_PaletteCamMover{m_PaletteCam};

};
