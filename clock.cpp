#include <windowsx.h>
#include "clock.h"

enum LCD
{
	LCD_TOP,
	LCD_MIDDLE,
	LCD_BOTTOM,
	LCD_TOP_LEFT,
	LCD_TOP_RIGHT,
	LCD_BOTTOM_LEFT,
	LCD_BOTTOM_RIGHT
};

WINDOW_BRUSH Active(__int64 value, ELEMENT_STATE state) noexcept
{
	if (value < 0)
		return WB_MAIN;
	else
	{
		switch(state)
		{
			case ES_CLICK: return WB_CLICK;
			case ES_HOVER: return WB_HOVER;
			default: return WB_SHADOW;
		}
	}
}
WINDOW_BRUSH Active(LCD elem, char value, ELEMENT_STATE state, bool pause_val) noexcept
{
	WINDOW_BRUSH vT = WB_MAIN;
	WINDOW_BRUSH vF;
	switch(state)
	{
		case ES_CLICK: vF = WB_CLICK; break;
		case ES_HOVER: vF = WB_HOVER; break;
		default: vF = WB_SHADOW; break;
	}

	if (pause_val)
		return vF;

	switch(value)
	{
		case 0:
			switch(elem)
			{
				case LCD_MIDDLE: return vF;
				default: return vT;
			}
		case 1:
			switch(elem)
			{
				case LCD_TOP_RIGHT:
				case LCD_BOTTOM_RIGHT: return vT;
				default: return vF;
			}
		case 2:
			switch(elem)
			{
				case LCD_TOP_LEFT:
				case LCD_BOTTOM_RIGHT: return vF;
				default: return vT;
			}
		case 3:
			switch(elem)
			{
				case LCD_TOP_LEFT:
				case LCD_BOTTOM_LEFT: return vF;
				default: return vT;
			}
		case 4:
			switch(elem)
			{
				case LCD_TOP:
				case LCD_BOTTOM:
				case LCD_BOTTOM_LEFT: return vF;
				default: return vT;
			}
		case 5:
			switch(elem)
			{
				case LCD_TOP_RIGHT:
				case LCD_BOTTOM_LEFT: return vF;
				default: return vT;
			}
		case 6:
			switch(elem)
			{
				case LCD_TOP_RIGHT: return vF;
				default: return vT;
			}
		case 7:
			switch(elem)
			{
				case LCD_TOP:
				case LCD_TOP_RIGHT:
				case LCD_BOTTOM_RIGHT: return vT;
				default: return vF;
			}
		case 8:
			return vT;
		case 9:
			switch(elem)
			{
				case LCD_BOTTOM_LEFT: return vF;
				default: return vT;
			}
	}
	return WB_MAIN;
}

void CLOCK::Paint() const noexcept
{
	using namespace D2D1;

	switch(state)
	{
		case ES_HOVER: parent->Target()->FillRectangle(rect, parent->Brush(WB_HOVER)); break;
		case ES_CLICK: parent->Target()->FillRectangle(rect, parent->Brush(WB_CLICK)); break;
	}

	parent->Target()->DrawRectangle(rect, parent->Brush(WB_MAIN), 2);

	const float stroke = 2.5f;

	const float x = rect.left*0.83f+rect.right*0.17f;
	const float cdx1 = (rect.right - rect.left)*0.01f;
	const float cdx2 = (rect.right - rect.left)*0.06f;
	const float cdx3 = (rect.right - rect.left)*0.12f;
	const float cdx4 = (rect.right - rect.left)*0.28f;

	const float yt1 = rect.top*0.85f+rect.bottom*0.15f;
	const float yt2 = rect.top*0.80f+rect.bottom*0.20f;
	const float ym1 = rect.top*0.55f+rect.bottom*0.45f;
	const float ym = rect.top*0.5f+rect.bottom*0.5f;
	const float ym2 = rect.top*0.45f+rect.bottom*0.55f;
	const float yb2 = rect.top*0.20f+rect.bottom*0.80f;
	const float yb1 = rect.top*0.15f+rect.bottom*0.85f;

	parent->Target()->DrawLine(Point2F(rect.left*0.96f+rect.right*0.04f, ym),
							   Point2F(rect.left*0.90f+rect.right*0.10f, ym), parent->Brush(Active(value, state)), stroke);

	char v[6];
	v[0] = abs(value)/36000%10;
	v[1] = abs(value)/3600%10;
	v[2] = abs(value)/60%60/10;
	v[3] = abs(value)/60%10;
	v[4] = abs(value)%60/10;
	v[5] = abs(value)%10;

	for (unsigned i = 0; i < 3; i++)
	{
		for (unsigned j = 0; j < 2; j++)
		{
			parent->Target()->DrawLine(Point2F(x+i*cdx4+j*cdx3, yt2),
									   Point2F(x+i*cdx4+j*cdx3, ym1),
									   parent->Brush(Active(LCD_TOP_LEFT, v[2*i+j], state, pause_val)), stroke);
			parent->Target()->DrawLine(Point2F(x+i*cdx4+j*cdx3, ym2),
									   Point2F(x+i*cdx4+j*cdx3, yb2),
									   parent->Brush(Active(LCD_BOTTOM_LEFT, v[2*i+j], state, pause_val)), stroke);

			parent->Target()->DrawLine(Point2F(x+i*cdx4+j*cdx3+(2*cdx1+cdx2), yt2),
									   Point2F(x+i*cdx4+j*cdx3+(2*cdx1+cdx2), ym1),
									   parent->Brush(Active(LCD_TOP_RIGHT, v[2*i+j], state, pause_val)), stroke);
			parent->Target()->DrawLine(Point2F(x+i*cdx4+j*cdx3+(2*cdx1+cdx2), ym2),
									   Point2F(x+i*cdx4+j*cdx3+(2*cdx1+cdx2), yb2),
									   parent->Brush(Active(LCD_BOTTOM_RIGHT, v[2*i+j], state, pause_val)), stroke);

			parent->Target()->DrawLine(Point2F(x+i*cdx4+j*cdx3+cdx1, yt1),
									   Point2F(x+i*cdx4+j*cdx3+cdx1+cdx2, yt1),
									   parent->Brush(Active(LCD_TOP, v[2*i+j], state, pause_val)), stroke);
			parent->Target()->DrawLine(Point2F(x+i*cdx4+j*cdx3+cdx1, ym),
									   Point2F(x+i*cdx4+j*cdx3+cdx1+cdx2, ym),
									   parent->Brush(Active(LCD_MIDDLE, v[2*i+j], state, pause_val)), stroke);
			parent->Target()->DrawLine(Point2F(x+i*cdx4+j*cdx3+cdx1, yb1),
									   Point2F(x+i*cdx4+j*cdx3+cdx1+cdx2, yb1),
									   parent->Brush(Active(LCD_BOTTOM, v[2*i+j], state, pause_val)), stroke);
		}

		if (i < 2)
		{
			parent->Target()->DrawLine(Point2F(x+cdx1+i*cdx4+(cdx4+cdx3+cdx2)/2, (yt2+ym1)/2),
									   Point2F(x+cdx1+i*cdx4+(cdx4+cdx3+cdx2)/2, (yt2+ym1)/2+stroke), parent->Brush(WB_MAIN), stroke);
			parent->Target()->DrawLine(Point2F(x+cdx1+i*cdx4+(cdx4+cdx3+cdx2)/2, (yb1+ym2)/2-stroke),
									   Point2F(x+cdx1+i*cdx4+(cdx4+cdx3+cdx2)/2, (yb1+ym2)/2), parent->Brush(WB_MAIN), stroke);
		}
	}
}
bool CLOCK::Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
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
						PostMessage(parent->Hwnd(), WM_COMMAND, 0, ET_CLOCK);
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
void CLOCK::SetValue(__int64 new_value) noexcept
{
	bool redraw = value != new_value;

	value = new_value;

	if (redraw)
		RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}

void CLOCK::Pause(bool arg) noexcept
{
	pause_val = arg;

	switch(arg)
	{
		case true: SetTimer(parent->Hwnd(), TIMER_ID, elapse, 0); break;
		case false: KillTimer(parent->Hwnd(), TIMER_ID); break;
	}
}
void CLOCK::Timer(WPARAM wParam) noexcept
{
	if (wParam != TIMER_ID)
		return;

	pause_val = !pause_val;
	RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}