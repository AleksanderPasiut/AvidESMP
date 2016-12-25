#pragma once

#include <Windows.h>

#include "graphics.h"
#include "elements.h"

enum INTERFACE_MODE
{
	IM_OUT = -2,
	IM_NONE = -1,
	IM_TOP_LEFT = 0,
	IM_TOP = 1,
	IM_TOP_RIGHT = 2,
	IM_LEFT = 3,
	IM_RIGHT = 4,
	IM_BOTTOM_LEFT = 5,
	IM_BOTTOM = 6,
	IM_BOTTOM_RIGHT = 7,
	IM_RIM = 8
};

enum WINDOW_BRUSH
{
	WB_MAIN = 0,
	WB_CLICK = 1,
	WB_HOVER = 2,
	WB_SHADOW = 3,
	WB_TRANSLUSCENT = 4,
};

class WINDOW
{
	static GRAPHICS* Graphics;

	WNDCLASSEXW wc;
	HWND hwnd;
	ID2D1HwndRenderTarget* target;

	bool tracking;
	TRACKMOUSEEVENT tme;

	D2D1_COLOR_F background;
	ID2D1SolidColorBrush* brush[5];
	float rim;
	float margin;
	float shw;
	void InitInternalPainting();
	void UpdateInternalPainting(D2D1_COLOR_F interface_color, D2D1_COLOR_F background_color) noexcept;
	void FreeInternalPainting() noexcept;
	void InternalPainting() noexcept;
	void InitPaint() noexcept;
	void EndPaint() noexcept;

	HCURSOR hcursor[6];
	void InitCursors();
	void FreeCursors() noexcept;
	void SetCursor(INTERFACE_MODE im) const noexcept;

	INTERFACE_MODE interface_mode;
	bool click;
	bool resizable;
	RECT init_rect;
	POINT init_click;
	POINT min_size;
	void InitInterface() noexcept;
	INTERFACE_MODE RetMode(D2D1_POINT_2F pt) const noexcept;
	void InitSizingNMoving() noexcept;
	void SizingNMoving() noexcept;
	bool EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	WINDOW(WNDPROC proc, DWORD ex_style, const wchar_t* class_name, const wchar_t* window_name, int x, int y, int width, int height, HWND parent = 0);
	WINDOW(const WINDOW&) = delete;
	WINDOW& operator= (const WINDOW&) = delete;
	~WINDOW() noexcept;

	void SetResizable(bool arg) noexcept { resizable = arg; }

	void SetRect(const RECT* rect) noexcept;
	void GetRect(RECT* rect) const noexcept;

public:
	HWND Hwnd() const noexcept { return hwnd; }
	ID2D1Factory* D2D1Factory() const noexcept { return Graphics->d2d1Factory; }
	IDWriteFactory* DWriteFactory() const noexcept { return Graphics->dwriteFactory; }
	ID2D1HwndRenderTarget* Target() const noexcept { return target; }
	ID2D1SolidColorBrush* Brush(WINDOW_BRUSH id) const noexcept { return brush[id]; }
	D2D1_COLOR_F RetInterfaceColor() const noexcept { return brush[WB_MAIN]->GetColor(); }
	D2D1_COLOR_F RetBackgroundColor() const noexcept { return background; }

	friend class CONTROLS;
};