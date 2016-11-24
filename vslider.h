#pragma once

#include "window.h"
#include "playlist_display.h"

class VSLIDER
{
	WINDOW* parent;
	ELEMENT_STATE state;
	D2D1_RECT_F rect;
	PLAYLIST_DISPLAY* display;

	float start_shift;
	D2D1_POINT_2F start;

	VSLIDER(WINDOW* parent, PLAYLIST_DISPLAY* display) : parent(parent), state(ES_NONE), display(display) {}
	VSLIDER(const VSLIDER&) = delete;
	VSLIDER& operator= (const VSLIDER&) = delete;

	D2D1_RECT_F Handle() const noexcept;
	void SetNewShift(float px, float py) const noexcept;
	void SetRect(const D2D1_RECT_F& rect) noexcept;
	void Paint() const noexcept;
	bool Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	friend class CONTROLS;
};