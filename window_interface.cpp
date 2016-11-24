#include <windowsx.h>

#include "window.h"

void WINDOW::InitInterface() noexcept
{
	interface_mode = IM_NONE;
	resizable = true;
	click = false;
	min_size = { 500, 150 };
}
INTERFACE_MODE WINDOW::RetMode(D2D1_POINT_2F pt) const noexcept
{
	INTERFACE_MODE im = IM_OUT;

	D2D1_SIZE_F size = target->GetSize();

	if (0 <= pt.x && pt.x < size.width && 
		0 <= pt.y && pt.y < size.height)
	{
		if (pt.x <= margin)
		{
			if (pt.y <= margin)
				im = IM_TOP_LEFT;
			else if (size.height-margin <= pt.y)
				im = IM_BOTTOM_LEFT;
			else if ((size.height-shw)/2 <= pt.y && pt.y <= (size.height+shw)/2)
				im = IM_LEFT;
			else im = IM_RIM;
		}
		else if (size.width - margin <= pt.x)
		{
			if (pt.y <= margin)
				im = IM_TOP_RIGHT;
			else if (size.height-margin <= pt.y)
				im = IM_BOTTOM_RIGHT;
			else if ((size.height-shw)/2 <= pt.y && pt.y <= (size.height+shw)/2)
				im = IM_RIGHT;
			else im = IM_RIM;
		}
		else if (pt.y <= margin)
		{
			if ((size.width-shw)/2 <= pt.x && pt.x <= (size.width+shw)/2)
				im = IM_TOP;
			else im = IM_RIM;
		}
		else if (size.height-margin <= pt.y)
		{
			if ((size.width-shw)/2 <= pt.x && pt.x <= (size.width+shw)/2)
				im = IM_BOTTOM;
			else im = IM_RIM;
		}
		else im = IM_NONE;
	}

	if (!resizable && im > IM_NONE)
		im = IM_RIM;
	
	return im;
}
void WINDOW::InitSizingNMoving() noexcept
{
	GetWindowRect(hwnd, &init_rect);
	GetCursorPos(&init_click);
}
void WINDOW::SizingNMoving() noexcept
{
	POINT pt;
	GetCursorPos(&pt);

	pt.x -= init_click.x;
	pt.y -= init_click.y;

	int x, y, cx, cy;

	switch(interface_mode)
	{
		case IM_OUT:
		case IM_NONE: return; 
		case IM_RIM:
		{
			cx = init_rect.right-init_rect.left;
			cy = init_rect.bottom-init_rect.top;
			x = init_rect.left+pt.x;
			y = init_rect.top+pt.y;
			break;
		}
		case IM_TOP_LEFT:
		{
			cx = init_rect.right-init_rect.left-pt.x;
			cy = init_rect.bottom-init_rect.top-pt.y;
			x = (cx < min_size.x) ? init_rect.right-min_size.x : init_rect.left+pt.x;
			y = (cy < min_size.y) ? init_rect.bottom-min_size.y : init_rect.top+pt.y;
			break;
		}
		case IM_TOP:
		{
			cx = init_rect.right-init_rect.left;
			cy = init_rect.bottom-init_rect.top-pt.y;
			x = init_rect.left;
			y = (cy < min_size.y) ? init_rect.bottom-min_size.y : init_rect.top+pt.y;
			break;
		}
		case IM_TOP_RIGHT:
		{
			cx = init_rect.right-init_rect.left+pt.x;
			cy = init_rect.bottom-init_rect.top-pt.y;
			x = init_rect.left;
			y = (cy < min_size.y) ? init_rect.bottom-min_size.y : init_rect.top+pt.y;
			break;
		}
		case IM_LEFT:
		{
			cx = init_rect.right-init_rect.left-pt.x;
			cy = init_rect.bottom-init_rect.top;
			x = (cx < min_size.x) ? init_rect.right-min_size.x : init_rect.left+pt.x;
			y = init_rect.top;
			break;
		}
		case IM_RIGHT:
		{
			cx = init_rect.right-init_rect.left+pt.x;
			cy = init_rect.bottom-init_rect.top;
			x = init_rect.left;
			y = init_rect.top;
			break;
		}
		case IM_BOTTOM_LEFT:
		{
			cx = init_rect.right-init_rect.left-pt.x;
			cy = init_rect.bottom-init_rect.top+pt.y;
			x = (cx < min_size.x) ? init_rect.right-min_size.x : init_rect.left+pt.x;
			y = init_rect.top;
			break;
		}
		case IM_BOTTOM:
		{
			cx = init_rect.right-init_rect.left;
			cy = init_rect.bottom-init_rect.top+pt.y;
			x = init_rect.left;
			y = init_rect.top;
			break;
		}
		case IM_BOTTOM_RIGHT:
		{
			cx = init_rect.right-init_rect.left+pt.x;
			cy = init_rect.bottom-init_rect.top+pt.y;
			x = init_rect.left;
			y = init_rect.top;
			break;
		}
	}

	if (resizable)
	{
		if (cx < min_size.x)
			cx = min_size.x;
		if (cy < min_size.y)
			cy = min_size.y;

		target->Resize(D2D1::SizeU(cx, cy));
		SetWindowPos(hwnd, 0, x, y, cx, cy, SWP_NOZORDER | SWP_NOREDRAW);
	}
	else SetWindowPos(hwnd, 0, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOREDRAW);
}
bool WINDOW::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	using namespace D2D1;

	if (hwnd != this->hwnd)
		return false;		

	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			interface_mode = RetMode(Point2F(static_cast<float>(GET_X_LPARAM(lParam)),
											 static_cast<float>(GET_Y_LPARAM(lParam))));
			click = true;
			InitSizingNMoving();
			SetCapture(hwnd);
			SetCursor(interface_mode);
			RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (!tracking)
			{
				TrackMouseEvent(&tme);
				tracking = true;
			}

			SetCursor(interface_mode);

			if (click)
				SizingNMoving();
			else
			{
				INTERFACE_MODE new_interface_mode = RetMode(Point2F(static_cast<float>(GET_X_LPARAM(lParam)),
																	static_cast<float>(GET_Y_LPARAM(lParam))));

				bool redraw = (new_interface_mode != interface_mode);
				interface_mode = new_interface_mode;

				if (redraw)
					RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			interface_mode = RetMode(Point2F(static_cast<float>(GET_X_LPARAM(lParam)),
											 static_cast<float>(GET_Y_LPARAM(lParam))));

			RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			ReleaseCapture();
			click = false;
			break;
		}
		case WM_MOUSELEAVE:
		{
			tracking = false;

			INTERFACE_MODE new_interface_mode = IM_OUT;

			bool redraw = (new_interface_mode != interface_mode);
			interface_mode = new_interface_mode;

			if (redraw)
				RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);

			break;
		}
	}

	return false;
}