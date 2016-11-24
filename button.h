#pragma once

#include "window.h"

class BUTTON
{
protected:
	WINDOW* parent;
	E_TYPE type;
	ELEMENT_STATE state;
	D2D1_RECT_F rect;

	BUTTON(WINDOW* parent, const D2D1_RECT_F& rect) : parent(parent), state(ES_NONE), rect(rect) {}
	BUTTON(const BUTTON&) = delete;
	BUTTON& operator= (const BUTTON&) = delete;

	void SetRect(const D2D1_RECT_F& rect) noexcept;
	void SharedPainting() const noexcept;
	bool Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	friend class CONTROLS;
};

class BUTTON_CLOSE : BUTTON
{
	BUTTON_CLOSE(WINDOW* parent, const D2D1_RECT_F& rect);
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_MINIMIZE : BUTTON
{
	BUTTON_MINIMIZE(WINDOW* parent, const D2D1_RECT_F& rect);
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_SETTINGS : BUTTON
{
	BUTTON_SETTINGS(WINDOW* parent, const D2D1_RECT_F& rect);
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_PLAYLIST : BUTTON
{
	BUTTON_PLAYLIST(WINDOW* parent, const D2D1_RECT_F& rect);
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_PREVIOUS : BUTTON
{
	ID2D1PathGeometry* shape;

	BUTTON_PREVIOUS(WINDOW* parent, const D2D1_RECT_F& rect);
	~BUTTON_PREVIOUS() noexcept;
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_STOP : BUTTON
{
	BUTTON_STOP(WINDOW* parent, const D2D1_RECT_F& rect);
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_PLAY : BUTTON
{
	ID2D1PathGeometry* shape;

	BUTTON_PLAY(WINDOW* parent, const D2D1_RECT_F& rect);
	~BUTTON_PLAY() noexcept;
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_PAUSE : BUTTON
{
	BUTTON_PAUSE(WINDOW* parent, const D2D1_RECT_F& rect);
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_NEXT : BUTTON
{
	ID2D1PathGeometry* shape;

	BUTTON_NEXT(WINDOW* parent, const D2D1_RECT_F& rect);
	~BUTTON_NEXT() noexcept;
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_UP : BUTTON
{
	BUTTON_UP(WINDOW* parent);
	void Paint() const noexcept;

	friend class CONTROLS;
};
class BUTTON_DOWN : BUTTON
{
	BUTTON_DOWN(WINDOW* parent);
	void Paint() const noexcept;

	friend class CONTROLS;
};
