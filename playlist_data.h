#pragma once

#include <vector>
#include "track_data.h"

class PLAYLIST_DATA
{
	std::vector<TRACK_DATA*> tab;
	TRACK_DATA* played;

	bool clear_playlist_on_shutdown;

	PLAYLIST_DATA();
	PLAYLIST_DATA(const PLAYLIST_DATA&) = delete;
	PLAYLIST_DATA& operator= (const PLAYLIST_DATA&) = delete;
	~PLAYLIST_DATA() noexcept;

	int GetTrackPos(TRACK_DATA* pt) const noexcept;
	int Difference(TRACK_DATA* pt1, TRACK_DATA* pt2) const noexcept;
	void SetSelection(TRACK_DATA* pt1, TRACK_DATA* pt2) const noexcept;
	void SmartSelection(TRACK_DATA* pivot, TRACK_DATA* start, TRACK_DATA* end) const noexcept;
	bool MoveSelection(__int64 interval) noexcept;
	void ClearSelection() const noexcept;
	void DeleteSelection() noexcept;
	void Clear() noexcept;
	void SelectFromFolder() const noexcept;
	TRACK_DATA* RetFirstSelected() const noexcept;
	void IncPlayed() noexcept;
	void DecPlayed() noexcept;
	bool IsFirstTrackPlayed() const noexcept;
	void SetClearPlaylistOnShutdown(bool arg) noexcept { clear_playlist_on_shutdown = arg; }
	bool RetClearPlaylistOnShutdown() const noexcept { return clear_playlist_on_shutdown; }

	void Load(std::fstream&) noexcept;
	void Save(std::fstream&) noexcept;

	friend class PLAYLIST_CONTEXT_MENU;
	friend class PLAYLIST_DISPLAY;
	friend class PLAYLIST;
	friend class VSLIDER;
};
