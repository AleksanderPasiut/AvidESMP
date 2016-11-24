#include <windowsx.h>

#include "vslider.h"

D2D1_RECT_F VSLIDER::Handle() const noexcept
{
	if (!display->data || display->data->tab.size() == 0)
		return rect;

	// minimalny rozmiaru uchwytu
	const float mhs = 10; 

	float lines_amount = static_cast<float>(display->data->tab.size());
	float des_disp_height = lines_amount*display->h + (lines_amount+1)*display->m;
	float handle_size = max(mhs, (rect.bottom - rect.top) * min(1.0f, (display->rect.bottom - display->rect.top) / des_disp_height));
	float handle_pos = rect.top + (rect.bottom-handle_size-rect.top) * display->shift / (des_disp_height - display->rect.bottom + display->rect.top);
	return D2D1::RectF(rect.left, handle_pos, rect.right, handle_pos + handle_size);
}
void VSLIDER::SetNewShift(float px, float py) const noexcept
{
	float lines_amount = static_cast<float>(display->data->tab.size());
	float des_disp_height = lines_amount*display->h + (lines_amount+1)*display->m;

	display->shift = start_shift + des_disp_height / (rect.bottom - rect.top)*(py - start.y);

	if (display->shift > des_disp_height - (display->rect.bottom-display->rect.top))
		display->shift = des_disp_height - (display->rect.bottom-display->rect.top);

	if (display->shift < 0)
		display->shift = 0;
}
void VSLIDER::SetRect(const D2D1_RECT_F& rect) noexcept
{
	this->rect = rect;
	RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}
void VSLIDER::Paint() const noexcept
{
	using namespace D2D1;

	const float stroke = 2.0f;
	const float b = 2.0f;

	parent->Target()->DrawLine(Point2F(0.5f*rect.left+0.5f*rect.right-b, rect.top),
							   Point2F(0.5f*rect.left+0.5f*rect.right-b, rect.bottom), parent->Brush(WB_MAIN), stroke);
	parent->Target()->DrawLine(Point2F(0.5f*rect.left+0.5f*rect.right+b, rect.top),
							   Point2F(0.5f*rect.left+0.5f*rect.right+b, rect.bottom), parent->Brush(WB_MAIN), stroke);

	D2D1_RECT_F handle = Handle();

	switch(state)
	{
		case ES_CLICK: parent->Target()->FillRectangle(handle, parent->Brush(WB_CLICK)); break;
		case ES_HOVER: parent->Target()->FillRectangle(handle, parent->Brush(WB_HOVER)); break;
		default: parent->Target()->FillRectangle(handle, parent->Brush(WB_SHADOW)); break;
	}

	parent->Target()->DrawRectangle(handle, parent->Brush(WB_MAIN), stroke);
}
bool VSLIDER::Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	using namespace D2D1;

	if (hwnd != parent->Hwnd())
		return false;

	bool ret = false;
	float px = static_cast<float>(GET_X_LPARAM(lParam));
	float py = static_cast<float>(GET_Y_LPARAM(lParam));

	switch(uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			if (state == ES_HOVER)
			{
				start = { px, py };
				start_shift = display->shift;

				state = ES_CLICK;
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
				ret = true;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			D2D1_RECT_F handle = Handle();
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

			if (state == ES_CLICK)
			{
				SetNewShift(px, py);
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			D2D1_RECT_F handle = Handle();
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
