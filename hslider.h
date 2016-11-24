#pragma once

#include "window.h"

class HSLIDER
{
	WINDOW* parent;
	ELEMENT_STATE state;
	D2D1_RECT_F rect;
	float handle_width;
	float pos;
	bool active;

	float start_pos;
	D2D1_POINT_2F start;

	HSLIDER(WINDOW* parent, const D2D1_RECT_F& rect, float handle_width) : parent(parent), state(ES_NONE), rect(rect), handle_width(handle_width), pos(0), active(false) {}
	HSLIDER(const HSLIDER&) = delete;
	HSLIDER& operator= (const HSLIDER&) = delete;

	void Paint() const noexcept;
	bool Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	void Activate(bool arg) noexcept { active = arg; }
	void SetPos(float new_pos) noexcept;
	float RetPos() const noexcept { return pos; }

	friend class CONTROLS;
};