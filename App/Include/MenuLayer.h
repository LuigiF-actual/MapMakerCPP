#pragma once

#include <print>

#include "raylib.h"

#include "Core/Layer.h"
#include "Core/Application.h"
#include "Core/Event.h"

#include "AppLayer.h"
#include "Menu.hpp"

class MenuLayer : public Core::Layer
{
public:
	MenuLayer() = default;
	~MenuLayer() override = default;

	void OnUpdate() override;
	void OnRender() override;

private:
	void OnEvent(Core::Event& event) override;

	bool OnKeyPressed(Core::KeyPressedEvent& event);

	bool OnWindowResize(Core::WindowResizeEvent& event);

private:
	Menu m_Menu;
};