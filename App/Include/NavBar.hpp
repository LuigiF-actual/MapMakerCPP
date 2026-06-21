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

        GuiSetIconScale(1);
	}


private:

	Rectangle m_Gbox = { 0.0f,0.0f,static_cast<float>(GetScreenWidth()),GetScreenHeight() * 0.05f };
	
	bool& m_Undo;
	bool& m_Redo;

	PaintMode& m_Pmode;

    int Active = 0;
};

