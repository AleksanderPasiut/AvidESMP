#pragma once

#include <Windows.h>

#include "player.h"
#include "playlist.h"
#include "controls.h"
#include "settings.h"

class PROGRAM
{
	static HANDLE hEvent;
	static bool PreOpen() noexcept;

	PLAYER Player;
	PLAYLIST Playlist;
	CONTROLS Controls;
	SETTINGS Settings;

	PROGRAM(WNDPROC proc);
	PROGRAM(const PROGRAM&) = delete;
	PROGRAM& operator= (const PROGRAM&) = delete;
	~PROGRAM() noexcept;

	LRESULT EventProc(HWND, UINT, WPARAM, LPARAM) noexcept;

	friend LRESULT CALLBACK WindowEventProc(HWND, UINT, WPARAM, LPARAM);
	friend int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
};