#pragma once

#include <Windows.h>
#include "playlist_data.h"

class PLAYLIST
{
	static const unsigned WM_TRANSFER = 0x8001;
	static const unsigned WM_DROPFILESLOADED = 0x8004;

	PLAYLIST_DATA data;

	bool clear_playlist_on_direct_file_open;
	bool clear_playlist_on_shutdown;

	PLAYLIST();
	PLAYLIST(const PLAYLIST&) = delete;
	PLAYLIST operator= (const PLAYLIST&) = delete;
	~PLAYLIST() noexcept;

	// playlist.cpp
	void SetMainHwnd(HWND) const noexcept;
	void Clear() noexcept;
	TRACK_DATA* LoadFilesFromCommandLine() noexcept;
	TRACK_DATA* Transfer(WPARAM wParam) noexcept;
	void SetFirstPlayed() noexcept;
	void SetPlayed(TRACK_DATA* played) noexcept { data.played = played; }
	void NoPlayed() noexcept { data.played = 0; }
	TRACK_HANDLE* RetPlayedHandle() const noexcept { return data.played ? &data.played->handle : 0; }
	TRACK_DATA* RetFirstSelected() const noexcept { return data.RetFirstSelected(); }
	void ClearSelection() const noexcept { data.ClearSelection(); }
	void IncPlayed() noexcept { data.IncPlayed(); }
	void DecPlayed() noexcept { data.DecPlayed(); }
	bool IsFirstTrackPlayed() const noexcept { return data.IsFirstTrackPlayed(); }
	void SetClearPlaylistOnDirectFileOpen(bool arg) noexcept { clear_playlist_on_direct_file_open = arg; }
	bool RetClearPlaylistOnDirectFileOpen() const noexcept { return clear_playlist_on_direct_file_open; }
	void SetClearPlaylistOnShutdown(bool arg) noexcept { data.SetClearPlaylistOnShutdown(arg); }
	bool RetClearPlaylistOnShutdown() const noexcept { return data.RetClearPlaylistOnShutdown(); }

	// playlist_drag_n_drop.cpp
	void StartDragNDrop(HDROP hDrop) const noexcept;
	friend DWORD WINAPI DragNDropThreadFunction(void*) noexcept;
	void FinishDragNDrop(WPARAM wParam, LPARAM lParam) noexcept;

	// playlist_load_save
	void Load(std::fstream& FS) noexcept { data.Load(FS); }
	void Save(std::fstream& FS) noexcept { data.Save(FS); }

	friend class CONTROLS;
	friend class SETTINGS;
	friend class PROGRAM;
};

struct DND_DATA
{
	HWND hwnd;	// static TRACK_DATA::hwnd

	HDROP hDrop;	// wejœcie
	std::vector<TRACK_DATA*>* inserted; // wyjœcie

	DND_DATA(HWND hwnd, HDROP hDrop) : hwnd(hwnd), hDrop(hDrop), inserted(0) {}
};
