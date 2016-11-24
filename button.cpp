#include <windowsx.h>

#include "button.h"

void BUTTON::SetRect(const D2D1_RECT_F& rect) noexcept
{
	this->rect = rect;
	RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}
void BUTTON::SharedPainting() const noexcept
{
	switch(state)
	{
		case ES_HOVER: parent->Target()->FillRectangle(rect, parent->Brush(WB_HOVER)); break;
		case ES_CLICK: parent->Target()->FillRectangle(rect, parent->Brush(WB_CLICK)); break;
	}

	parent->Target()->DrawRectangle(rect, parent->Brush(WB_MAIN), 2);
}
bool BUTTON::Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (hwnd != parent->Hwnd())
		return false;

	bool ret = false;
	float px = static_cast<float>(GET_X_LPARAM(lParam));
	float py = static_cast<float>(GET_Y_LPARAM(lParam));

	switch (uMsg)
	{
		case WM_LBUTTONDOWN:
		{
			if (state == ES_HOVER)
			{
				state = ES_CLICK;
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
				ret = true;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (rect.left <= px && px <= rect.right && rect.top <= py && py <= rect.bottom)
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

			break;
		}
		case WM_LBUTTONUP:
		{
			if (rect.left <= px && px <= rect.right && rect.top <= py && py <= rect.bottom)
			{
				switch(state)
				{
					case ES_CLICK:
					{
						state = ES_HOVER;
						RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
						ret = true;
						PostMessage(parent->Hwnd(), WM_COMMAND, 0, static_cast<LPARAM>(type));
						break;
					}
					case ES_NONE:
					{
						state = ES_HOVER;
						RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
						ret = false;
						break;
					}
				}
			}
			else
			{
				state = ES_NONE;
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
				ret = false;
			}
			break;
		}
		case WM_MOUSELEAVE:
		{
			state = ES_NONE;
			RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			break;
		}
	}

	return ret;
}