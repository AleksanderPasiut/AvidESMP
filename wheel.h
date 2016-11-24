#pragma once

#include "window.h"

class WHEEL
{
	WINDOW* parent;
	ELEMENT_STATE state;
	D2D1_POINT_2F center;
	float radius;
	float pos;

	const float stroke;
	const float m1;
	const float m2;
	const float m3;
	const float m4;
	const unsigned ma;
	const float alpha;
	const float phi;

	D2D1_POINT_2F start;

	WHEEL(WINDOW* parent, const D2D1_POINT_2F& center, float radius);
	WHEEL(const WHEEL&) = delete;
	WHEEL& operator= (const WHEEL&) = delete;

	bool In(float px, float py) const noexcept;
	void Paint() const noexcept;
	bool Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	void SetPos(float new_pos) noexcept;
	float RetPos() const noexcept { return pos; }

	friend class CONTROLS;
};