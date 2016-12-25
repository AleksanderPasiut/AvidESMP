#pragma once

#include "graphics.h"
#include "window.h"
#include "button.h"
#include "clock.h"
#include "wheel.h"
#include "hslider.h"
#include "playlist_display.h"
#include "vslider.h"
#include "dnd_loading.h"

#include "player.h"
#include "playlist.h"

class CONTROLS
{
	GRAPHICS Graphics;

	WINDOW* MainWindow;
	WINDOW* ListWindow;

	BUTTON_CLOSE* ButtonClose;
	BUTTON_MINIMIZE* ButtonMinimize;
	BUTTON_SETTINGS* ButtonSettings;
	BUTTON_PLAYLIST* ButtonPlaylist;

	BUTTON_PREVIOUS* ButtonPrevious;
	BUTTON_STOP* ButtonStop;
	BUTTON_PLAY* ButtonPlay;
	BUTTON_PAUSE* ButtonPause;
	BUTTON_NEXT* ButtonNext;

	CLOCK* Clock;
	WHEEL* Wheel;
	HSLIDER* HSlider;

	PLAYLIST_DISPLAY* PlaylistDisplay;
	DND_LOADING* DndLoading;
	VSLIDER* VSlider;
	BUTTON_UP* ButtonLineUp;
	BUTTON_DOWN* ButtonLineDown;

	PLAYER* Player;
	PLAYLIST* Playlist;

	static const DWORD TIMER_ID = 0x101;
	static const DWORD elapse = 100;

	bool autoplay_on_direct_file_open;
	bool repeat;

	bool show_list_window;

	CONTROLS(WNDPROC proc, PLAYER*, PLAYLIST*);
	CONTROLS(const CONTROLS&) = delete;
	CONTROLS& operator= (const CONTROLS&) = delete;
	~CONTROLS() noexcept;

	void ShowWindows() const noexcept;
	void AutoPlay(TRACK_DATA*) noexcept;
	void PlaceListWindowElements() const noexcept;
	bool EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
	void ProcessCommand(WPARAM wParam, LPARAM lParam) noexcept;
	void ProcessPrevious() noexcept;
	void ProcessStop() noexcept;
	void ProcessPlaylistDisplay() noexcept;
	void ProcessPlay() noexcept;
	void ProcessPause() noexcept;
	void ProcessNext() noexcept;
	void SetMainTimer(bool) noexcept;
	void ProcessTimer(WPARAM wParam) noexcept;
	void ProcessTrackEnd() noexcept;
	void ProcessHSlider(E_TYPE msg) noexcept;

	std::vector<TRACK_DATA*>::iterator RetIteratorFromPoint(const POINT* pt) const noexcept { return PlaylistDisplay->RetIteratorFromPoint(pt); }
	HWND RetMainHwnd() const noexcept { return MainWindow->Hwnd(); }

	D2D1_COLOR_F RetInterfaceColor() const noexcept { return MainWindow->RetInterfaceColor(); }
	D2D1_COLOR_F RetBackgroundColor() const noexcept { return MainWindow->RetBackgroundColor(); }
	void SetColors(D2D1_COLOR_F interface_color, D2D1_COLOR_F background_color) const noexcept { MainWindow->UpdateInternalPainting(interface_color, background_color); ListWindow->UpdateInternalPainting(interface_color, background_color); }
	bool RetAutoPlayOnDirectFileOpen() const noexcept { return autoplay_on_direct_file_open; }
	void SetAutoPlayOnDirectFileOpen(bool arg) noexcept { autoplay_on_direct_file_open = arg; }
	bool RetRepeat() const noexcept { return repeat; }
	void SetRepeat(bool arg) noexcept { repeat = arg; }

	// controls_load_save.cpp
	void Load(std::fstream&) noexcept;
	void Save(std::fstream&) const noexcept;

	friend class SETTINGS;
	friend class PROGRAM;
};