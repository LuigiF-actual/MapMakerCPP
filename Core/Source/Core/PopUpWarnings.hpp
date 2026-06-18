#pragma once

#include <tinyfiledialogs.h>


namespace Core::Warnings
{
	inline void beep()
	{
		tinyfd_beep();
	}

	inline int notifyPopUp(const char* title, const char* message, const char* iconType)
	{
		return tinyfd_notifyPopup(title, message, iconType);
	}

	inline int messageBox(const char* title, const char* message, const char* dialog, const char* iconType, int btn)
	{
		return tinyfd_messageBox(title, message, dialog, iconType, btn);
	}
}