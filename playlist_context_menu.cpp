#include <windowsx.h>

#include "playlist_context_menu.h"

PLAYLIST_CONTEXT_MENU::PLAYLIST_CONTEXT_MENU(HWND hwnd, PLAYLIST_DATA* data) : 
	hwnd(hwnd),
	data(data)
{
	hMotherMenu = LoadMenu(0, "context_menu");
	hMenu = GetSubMenu(hMotherMenu, 0);
}
PLAYLIST_CONTEXT_MENU::~PLAYLIST_CONTEXT_MENU() noexcept
{
	DestroyMenu(hMenu);
	DestroyMenu(hMotherMenu);
}

void PLAYLIST_CONTEXT_MENU::Mouse(WPARAM wParam, LPARAM lParam) const noexcept
{
	POINT pt;
	GetCursorPos(&pt);
	TrackPopupMenu(hMenu, TPM_LEFTBUTTON, pt.x, pt.y, 0, hwnd, 0);
}
bool PLAYLIST_CONTEXT_MENU::Command(WPARAM wParam, LPARAM lParam) noexcept
{
	if (HIWORD(wParam))
		return false;

	switch (LOWORD(wParam))
	{
		case CMM_PLAY:
		{
			return true;
		}
		case CMM_DELETE:
		{
			if (data->played && data->played->selected)
				SendMessage(hwnd, WM_COMMAND, 0, ET_STOP);

			data->DeleteSelection();
			break;
		}
		case CMM_CLEAR:
		{
			SendMessage(hwnd, WM_COMMAND, 0, ET_STOP);
			data->Clear();
			break;
		}
		case CMM_SELECT_FROM_FOLDER: data->SelectFromFolder(); break;
	}

	return false;
}