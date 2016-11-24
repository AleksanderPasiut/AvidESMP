#pragma once

#include "window.h"
#include "playlist_data.h"

#define CMM_PLAY 0x1
#define CMM_DELETE 0x2
#define CMM_CLEAR 0x3
#define CMM_SELECT_FROM_FOLDER 0x4

class PLAYLIST_CONTEXT_MENU
{
	HWND hwnd;

	HMENU hMotherMenu;
	HMENU hMenu;

	PLAYLIST_DATA* data;

	PLAYLIST_CONTEXT_MENU(HWND hwnd, PLAYLIST_DATA* data);
	PLAYLIST_CONTEXT_MENU(const PLAYLIST_CONTEXT_MENU&) = delete;
	PLAYLIST_CONTEXT_MENU& operator= (const PLAYLIST_CONTEXT_MENU&) = delete;
	~PLAYLIST_CONTEXT_MENU() noexcept;

	void Mouse(WPARAM, LPARAM) const noexcept;
	bool Command(WPARAM, LPARAM) noexcept;

	friend class PLAYLIST_DISPLAY;
};
