#pragma once

#include "playlist.h"
#include "controls.h"

#define CTRL_GB_BASIC_SETTINGS 0x101
#define CTRL_GB_SET_PROGRAM_COLORS 0x102
#define CTRL_CB_AUTO_PLAY_ON_DIRECT_FILE_OPEN 0x201
#define CTRL_CB_CLEAR_PLAYLIST_ON_DIRECT_FILE_OPEN 0x202
#define CTRL_CB_CLEAR_PLAYLIST_ON_SHUTDOWN 0x203
#define CTRL_CB_REPEAT 0x204
#define CTRL_PB_SET_BACKGROUND_COLOR 0x205
#define CTRL_PB_SET_INTERFACE_COLOR 0x206
#define CTRL_OK 0x301
#define CTRL_CANCEL 0x302

struct SETTINGS_SET
{
	D2D1_COLOR_F interface_color; // CONTROLS
	D2D1_COLOR_F background_color; // CONTROLS
	bool auto_play_on_direct_file_open; // CONTROLS
	bool clear_playlist_on_direct_file_open; // PLAYLIST
	bool clear_playlist_on_shutdown; // PLAYLIST
	bool repeat; // CONTROLS
};

class SETTINGS
{
	static SETTINGS* own;

	HWND hwnd_parent;
	PLAYLIST* playlist;
	CONTROLS* controls;

	std::wstring path;

	SETTINGS_SET set;

	static COLORREF cust_colors[16];

	SETTINGS(HWND hwnd_parent, PLAYLIST* playlist, CONTROLS* controls);
	SETTINGS(const SETTINGS&) = delete;
	SETTINGS& operator= (const SETTINGS&) = delete;
	~SETTINGS() noexcept;

	void InitSavingPath() noexcept;

	void RetrieveSet() noexcept;
	void Proceed() noexcept;
	void ApplySet() const noexcept;

	void LoadFile() noexcept;
	void SaveFile() noexcept;

	static void InitDialogValues(HWND) noexcept;
	static void ChangeAutoplayOnDirectFileOpen(HWND) noexcept;
	static void ChangeClearPlaylistOnDirectFileOpen(HWND) noexcept;
	static void ChangeClearPlaylistOnShutdown(HWND) noexcept;
	static void ChangeRepeat(HWND) noexcept;
	static void ChangeColorBackground(HWND) noexcept;
	static void ChangeColorInterface(HWND) noexcept;

	friend class PROGRAM;
	friend BOOL CALLBACK DialogSettingsProc(HWND, UINT, WPARAM, LPARAM);
};

BOOL CALLBACK DialogSettingsProc(HWND, UINT, WPARAM, LPARAM);