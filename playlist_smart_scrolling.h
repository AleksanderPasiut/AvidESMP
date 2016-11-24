#pragma once

#include "window.h"

class PLAYLIST_SMART_SCROLLING
{
	HWND hwnd;
	float& shift;
	const D2D1_RECT_F& rect;

	const UINT_PTR ID;
	const unsigned elapse;

	float speed;
	float margin;
	float ref_speed;

	PLAYLIST_SMART_SCROLLING(HWND, float& shift, const D2D1_RECT_F& rect);
	PLAYLIST_SMART_SCROLLING(const PLAYLIST_SMART_SCROLLING&) = delete;
	PLAYLIST_SMART_SCROLLING operator= (const PLAYLIST_SMART_SCROLLING&) = delete;

	void Set() noexcept;
	void Update(float px, float py) noexcept;
	bool Perform(WPARAM wParam) noexcept;
	void Finish() noexcept;

	friend class PLAYLIST_DISPLAY;
};
