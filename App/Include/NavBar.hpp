#pragma once

#include <raylib.h>
#include <raygui.h>


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

        ///if (GuiTabBar(Rectangle{ 100.0f + boxSize * 3, boxSize, boxSize, boxSize }, const_cast<char**>(m_Words), 2, &Active) == 1)
        //{

        //}

        GuiSetIconScale(1);
	}


private:

	Rectangle m_Gbox = { 0.0f,0.0f,static_cast<float>(GetScreenWidth()),GetScreenHeight() * 0.05f };
	
	bool& m_Undo;
	bool& m_Redo;

	PaintMode& m_Pmode;

    int Active = 0;
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