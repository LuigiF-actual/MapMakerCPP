#pragma once

#include <stdint.h>

#include "Core/Layer.h"
#include "Core/InputEvents.h"
#include <raylib.h>

class OverlayLayer : public Core::Layer
{
public:
	OverlayLayer();
	~OverlayLayer() override;

	void OnEvent(Core::Event& event) override;

	void OnUpdate() override;
	void OnRender() override;
private:
	bool IsButtonHovered() const;

	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);
private:
	Texture2D m_Texture;

	bool m_IsHovered = false;
	bool m_Pressed = true;
};