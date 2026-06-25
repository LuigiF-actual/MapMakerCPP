#pragma once

#include <raylib.h>
#include <raygui.h>
#include <iostream>

#include "PaintMode.hpp"

class NavBar
{
public:

    NavBar(PaintMode& mode, bool& redo, bool& undo)
        :
        m_Pmode(mode),
        m_Redo(redo),
        m_Undo(undo)
    {};
    
    ~NavBar() = default;



	void update()
	{
		GuiGroupBox({ 0.0f,0.0f,static_cast<float>(GetScreenWidth()),GetScreenHeight() * 0.05f }, "Tools");
        
        float boxSize = GetScreenHeight() * 0.05f;
        int iconScale = static_cast<int>(boxSize / 16.0f);

        if (iconScale < 1) iconScale = 1;

        GuiSetIconScale(iconScale);

        int tempActive = static_cast<int>(m_Pmode);

        GuiToggleGroup(Rectangle{ 100.0f + boxSize * 2, 0.0f, boxSize, boxSize }, "#22#;#25#;#29#", &tempActive);

        m_Pmode = static_cast<PaintMode>(tempActive);

        if (GuiButton(Rectangle{ 100.0f + boxSize * 6, 0.0f, boxSize, boxSize }, "#56#") == 1)
        {
            m_Undo = true;
        }

        if (GuiButton(Rectangle{ 100.0f + boxSize * 7, 0.0f, boxSize, boxSize }, "#57#") == 1)
        {
            m_Redo = true;
        }


        GuiCheckBox(Rectangle{ 100.0f + boxSize * 8, 0.0f, boxSize, boxSize }, "Textures Chooser", &m_IsTexSelectorActive);

        if (m_IsTexSelectorActive)
        {
            drawTextureSelector();
        }

        GuiSetIconScale(1);
	}


private:

    void drawTextureSelector()
    {
        for (auto const& dir_entry : std::filesystem::directory_iterator{ Config::getImagesDir() })
        {
            m_PanelContent.height += 120.0f;
        }

        GuiScrollPanel(m_WinBoxBody, "Choose Save", m_PanelContent, &m_PanelScroll, &m_PanelView);

        if (GuiButton(Rectangle{ 10.0f, 10.0f, m_WinBoxBody.width * 40.0f / 100 , m_WinBoxBody.height * 15.0f / 100 }, "Leave") == 1)
        {
            return;
        }

        BeginScissorMode(static_cast<int>(m_PanelView.x), static_cast<int>(m_PanelView.y), static_cast<int>(m_PanelView.width), static_cast<int>(m_PanelView.height));

        for (const auto [index, pathToSave] : std::views::enumerate(std::filesystem::directory_iterator{ Config::getImagesDir() }))
        {
            Rectangle itemRect = {
                m_WinBoxBody.x + m_PanelScroll.x + 20,				 // Base X + Scroll Offset + Padding
                m_WinBoxBody.y + m_PanelScroll.y + 20 + (index * 120), // Base Y + Scroll Offset + Padding + Spacing
                400,
                100
            };
            if (GuiButton(itemRect, pathToSave.path().stem().string().c_str()) == 1)
            {
                std::cout << std::filesystem::path(pathToSave.path()).append(pathToSave.path().stem().string()) << "\n";
                break;
            }
        }


        EndScissorMode();
        m_PanelContent.height = 0.0f;
    }

private:

	Rectangle m_Gbox = { 0.0f,0.0f,static_cast<float>(GetScreenWidth()),GetScreenHeight() * 0.05f };

    Rectangle m_TexNameBox = { GetScreenWidth() / 2.0f,GetScreenHeight() / 2.0f,static_cast<float>(GetScreenWidth() * 0.15),static_cast<float>(GetScreenHeight() * 0.15) };

    Rectangle m_WinBoxBody = { 400.0f, 100.0f, 500.0f, 500.0f };//{ GetScreenWidth() / 2.0f - 440.0f,50.0f, 440.0f,880.0f };
    Rectangle m_PanelContent = { 0.0f, 0.0f, 500.0f, 0.0f };    // The total size of the inside area
    Vector2 m_PanelScroll = { 0.0f, 0.0f };                       // Tracks X/Y scroll position
    Rectangle m_PanelView = { 0 };


	bool& m_Undo;
	bool& m_Redo;
    
    bool m_IsTexSelectorActive = false;

	PaintMode& m_Pmode;

    int m_Active = 0;
};

/*


int GuiRadioButton(Rectangle bounds, RadioBtnState* btnState, RadioBtnState btnValue, const char* text, bool* checked)
{
    int result = 0;
    GuiState state = guiState;

    bool temp = false;
    if (checked == NULL) checked = &temp;

    Rectangle textBounds = { 0 };

    if (text != NULL)
    {
        textBounds.width = (float)GuiGetTextWidth(text) + 2;
        textBounds.height = (float)GuiGetStyle(DEFAULT, TEXT_SIZE);
        textBounds.x = bounds.x + bounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING);
        textBounds.y = bounds.y + bounds.height / 2 - GuiGetStyle(DEFAULT, TEXT_SIZE) / 2;
        if (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) textBounds.x = bounds.x - textBounds.width - GuiGetStyle(CHECKBOX, TEXT_PADDING);
    }

    // Update control
    //--------------------------------------------------------------------
    if ((state != STATE_DISABLED) && !guiLocked && !guiControlExclusiveMode)
    {
        Vector2 mousePoint = GUI_POINTER_POSITION;

        Rectangle totalBounds = {
            (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_LEFT) ? textBounds.x : bounds.x,
            bounds.y,
            bounds.width + textBounds.width + GuiGetStyle(CHECKBOX, TEXT_PADDING),
            bounds.height,
        };

        // Check checkbox state
        if (CheckCollisionPointRec(mousePoint, totalBounds))
        {
            if (GUI_BUTTON_DOWN) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (GUI_BUTTON_RELEASED)
            {
                *checked = !(*checked);
                result = 1;
            }
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    GuiDrawRectangle(bounds, GuiGetStyle(CHECKBOX, BORDER_WIDTH), GetColor(GuiGetStyle(CHECKBOX, BORDER + (state * 3))), BLANK);

    if (*checked)
    {
        Rectangle check = { bounds.x + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.y + GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING),
                            bounds.width - 2 * (GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)),
                            bounds.height - 2 * (GuiGetStyle(CHECKBOX, BORDER_WIDTH) + GuiGetStyle(CHECKBOX, CHECK_PADDING)) };
        GuiDrawRectangle(check, 0, BLANK, GetColor(GuiGetStyle(CHECKBOX, TEXT + state * 3)));
    }

    GuiDrawText(text, textBounds, (GuiGetStyle(CHECKBOX, TEXT_ALIGNMENT) == TEXT_ALIGN_RIGHT) ? TEXT_ALIGN_LEFT : TEXT_ALIGN_RIGHT, GetColor(GuiGetStyle(LABEL, TEXT + (state * 3))));
    //--------------------------------------------------------------------

    return result;
}



*/