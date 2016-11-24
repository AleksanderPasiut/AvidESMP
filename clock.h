#pragma once

#include "window.h"

class CLOCK
{
	WINDOW* parent;
	ELEMENT_STATE state;
	D2D1_RECT_F rect;
	__int64 value;

	static const DWORD TIMER_ID = 0x103;
	static const DWORD elapse = 500;
	bool pause_val;

	CLOCK(WINDOW* parent, const D2D1_RECT_F& rect)  : parent(parent), state(ES_NONE), rect(rect), value(0) {}
	CLOCK(const CLOCK&) = delete;
	CLOCK& operator= (const CLOCK&) = delete;

	void Paint() const noexcept;
	bool Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	void Pause(bool) noexcept;
	void Timer(WPARAM wParam) noexcept;

	void SetValue(__int64 new_value) noexcept;
	__int64 RetValue() const noexcept { return value; }

	friend class CONTROLS;
};