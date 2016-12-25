#pragma once

#include "window.h"
#include "playlist_display.h"

class DND_LOADING
{
	WINDOW* parent;
	PLAYLIST_DISPLAY* display;
	unsigned factor;

	float angle;
	static const unsigned TIMER_ID = 0x102;
	static const unsigned elapse = 20;

	DND_LOADING(WINDOW* parent, PLAYLIST_DISPLAY* display);
	DND_LOADING(const DND_LOADING&) = delete;
	DND_LOADING& operator= (const DND_LOADING&) = delete;

	void IncFactor() noexcept;
	void DecFactor() noexcept;
	void Timer(WPARAM wParam) noexcept;
	void Paint() const noexcept;
	
	friend class CONTROLS;
};