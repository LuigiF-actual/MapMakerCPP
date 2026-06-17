#pragma once

#include <tinyfiledialogs.h>


namespace Core
{
	class PopUpWarnings
	{
	public:
		PopUpWarnings() = default;

		~PopUpWarnings() = default;

		void beep()
		{
			tinyfd_beep();
		}

		int notifyPopUp(const char* title, const char* message, const char* iconType)
		{
			return tinyfd_notifyPopup(title, message, iconType);
		}

		int messageBox(const char* title, const char* message, const char* dialog, const char* iconType, int btn)
		{
			return tinyfd_messageBox(title, message, dialog, iconType, btn);
		}

	private:

	};

}