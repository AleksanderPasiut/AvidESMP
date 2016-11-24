#include <windowsx.h>

#include "hslider.h"

void HSLIDER::Paint() const noexcept
{
	using namespace D2D1;

	const float stroke = 2.0f;
	const float b = 2.0f;

	parent->Target()->DrawLine(Point2F(rect.left, 0.5f*rect.top+0.5f*rect.bottom-b),
							   Point2F(rect.right, 0.5f*rect.top+0.5f*rect.bottom-b), parent->Brush(WB_MAIN), stroke);
	parent->Target()->DrawLine(Point2F(rect.left, 0.5f*rect.top+0.5f*rect.bottom+b),
							   Point2F(rect.right, 0.5f*rect.top+0.5f*rect.bottom+b), parent->Brush(WB_MAIN), stroke);

	D2D1_RECT_F handle = RectF(rect.left+(rect.right-rect.left-handle_width)*pos,
							   rect.top,
							   rect.left+handle_width+(rect.right-rect.left-handle_width)*pos,
							   rect.bottom);

	switch(state)
	{
		case ES_CLICK: parent->Target()->FillRectangle(handle, parent->Brush(WB_CLICK)); break;
		case ES_HOVER: parent->Target()->FillRectangle(handle, parent->Brush(WB_HOVER)); break;
		default: parent->Target()->FillRectangle(handle, parent->Brush(WB_SHADOW)); break;
	}

	parent->Target()->DrawRectangle(handle, parent->Brush(WB_MAIN), stroke);
}
bool HSLIDER::Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	using namespace D2D1;

	if (hwnd != parent->Hwnd())
		return false;

	bool ret = false;
	float px = static_cast<float>(GET_X_LPARAM(lParam));
	float py = static_cast<float>(GET_Y_LPARAM(lParam));

	D2D1_RECT_F handle = RectF(rect.left+(rect.right-rect.left-handle_width)*pos,
							   rect.top,
							   rect.left+handle_width+(rect.right-rect.left-handle_width)*pos,
							   rect.bottom);
	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			if (state == ES_HOVER)
			{
				start_pos = pos;
				start = { px, py };

				state = ES_CLICK;
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);

				if (active)
					PostMessage(parent->Hwnd(), WM_COMMAND, 0, ET_HSLIDER_START);

				ret = true;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (handle.left <= px && px <= handle.right && handle.top <= py && py <= handle.bottom)
			{
				if (state == ES_NONE && !(wParam & MK_LBUTTON))
				{
					state = ES_HOVER;
					RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
					ret = true;
				}
			}
			else
			{
				if (state == ES_HOVER)
				{
					state = ES_NONE;
					RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
					ret = true;
				}
			}

			if (active && state == ES_CLICK)
			{
				pos = start_pos + (px - start.x)/(rect.right-rect.left-handle_width);

				if (pos < 0)
					pos = 0;
				else if (pos > 1)
					pos = 1;

				PostMessage(parent->Hwnd(), WM_COMMAND, 0, ET_HSLIDER_MOVE);
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			if (active && state == ES_CLICK)
				PostMessage(parent->Hwnd(), WM_COMMAND, 0, ET_HSLIDER_STOP);

			if (handle.left <= px && px <= handle.right && handle.top <= py && py <= handle.bottom)
				state = ES_HOVER;
			else state = ES_NONE;

			break;
		}
		case WM_MOUSELEAVE:
		{
			state = ES_NONE;
			RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			break;
		}
	}

	return false;
}

void HSLIDER::SetPos(float new_pos) noexcept
{
	bool redraw = new_pos != pos;

	pos = new_pos;

	if (redraw)
		RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}