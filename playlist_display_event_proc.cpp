#include <windowsx.h>
#include "playlist_display.h"

enum KEY_SET
{
	KS_CTRL_SHIFT,
	KS_SHIFT,
	KS_CTRL,
	KS_NONE
};
KEY_SET WParamToKeySet(WPARAM wParam) noexcept
{
	if (wParam & MK_CONTROL)
	{
		if (wParam & MK_SHIFT)
			return KS_CTRL_SHIFT;
		else return KS_CTRL;
	}
	else if (wParam & MK_SHIFT)
		return KS_SHIFT;
	else return KS_NONE;

}

TRACK_DATA* PLAYLIST_DISPLAY::RetTrackFromPointHard(float px, float py) const noexcept
{
	if (rect.left+m < px && px < rect.right-m)
	{
		unsigned i = static_cast<unsigned>(shift/(h+m));
		unsigned limit = min(static_cast<unsigned>(data->tab.size()), static_cast<unsigned>((shift+rect.bottom-rect.top)/(h+m))+1);
		for (auto it = data->tab.begin() + i; i < limit; i++, it++)
			if (rect.top+m+i*(h+m)-shift <= py && py <= rect.top+(i+1)*(h+m)-shift)
				return *it;
	}
	return 0;
}
TRACK_DATA* PLAYLIST_DISPLAY::RetTrackFromPointSoft(float px, float py) const noexcept
{
	if (py < rect.top)
		py = rect.top;
	else if (py > rect.bottom)
		py = rect.bottom;

	unsigned hit = static_cast<unsigned>((py-rect.top+shift+m/2)/(h+m));

	if (hit < static_cast<unsigned>(data->tab.size()))
		return *(data->tab.begin()+hit);
	else return 0;
}
std::vector<TRACK_DATA*>::iterator PLAYLIST_DISPLAY::RetIteratorFromPoint(const POINT* pt) const noexcept
{
	float py = static_cast<float>(pt->y);

	int hit = static_cast<int>((py-rect.top+shift+m/2)/(h+m));

	if (hit < 0)
		return data->tab.begin();
	else if (hit >= data->tab.size())
		return data->tab.end();
	else return data->tab.begin() + hit;
}
void PLAYLIST_DISPLAY::PerformMoving(float px, float py) noexcept
{
	TRACK_DATA* current = RetTrackFromPointSoft(px, py);

	if (current)
	{
		int difference = data->Difference(current, marked);
		if (difference)
		{
			data->MoveSelection(difference);
			RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			moved = true;
		}
	}
}
void PLAYLIST_DISPLAY::PerformSelecting(float px, float py) noexcept
{
	TRACK_DATA* current = RetTrackFromPointSoft(px, py);

	if (current)
	{
		data->SmartSelection(last_selected, marked, current);
		marked = current;
		RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
	}
}
void PLAYLIST_DISPLAY::RButtonDown(float px, float py, WPARAM wParam, LPARAM lParam) noexcept
{
	if (marked)
	{
		if (!marked->selected)
		{
			data->ClearSelection();
			marked->selected = true;
		}
		last_selected = marked;
		SendMessage(parent->Hwnd(), WM_PAINT, 0, 0);
		ContextMenu.Mouse(wParam, lParam);
	}
}
void PLAYLIST_DISPLAY::LButtonDown(float px, float py, WPARAM wParam) noexcept
{
	if (rect.left <= px && px <= rect.right && rect.top <= py && py <= rect.bottom)
	{
		switch(WParamToKeySet(wParam))
		{
			case KS_CTRL_SHIFT:
			{
				if (marked)
				{
					if (last_selected)
						data->SetSelection(last_selected, marked);
					else
					{
						marked->selected = true;
						last_selected = marked;
					}
				}
				break;
			}
			case KS_CTRL:
			{
				if (marked)
				{
					marked->selected = !(marked->selected);
					if (marked->selected)
						last_selected = marked;
				}
				break;
			}
			case KS_SHIFT:
			{
				if (marked)
				{
					data->ClearSelection();

					if (last_selected)
						data->SetSelection(last_selected, marked);
					else
					{
						marked->selected = true;
						last_selected = marked;
					}
				}
				break;
			}
			case KS_NONE:
			{
				if (marked)
				{
					if (!marked->selected)
					{
						data->ClearSelection();
						marked->selected = true;
						last_selected = marked;
					}
					else to_be_played = true;
				}
				else
				{
					data->ClearSelection();
					last_selected = 0;
				}
				break;
			}
		}

		moved = false;
		clicked = true;
		SmartScrolling.Set();
	}
}
void PLAYLIST_DISPLAY::MouseMove(float px, float py, WPARAM wParam) noexcept
{
	bool redraw = false;

	if (clicked)
	{
		SmartScrolling.Update(px, py);

		if (marked && last_selected)
		{
			if (to_be_played)
				PerformMoving(px, py);
			else PerformSelecting(px, py);
		}
	}
	else if (!(wParam & MK_LBUTTON))
	{
		TRACK_DATA* new_marked = RetTrackFromPointHard(px, py);

		if (marked != new_marked)
		{
			redraw = true;
			marked = new_marked;
		}
	}
			
	if (redraw)
		RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}
void PLAYLIST_DISPLAY::LButtonUp(float px, float py, WPARAM wParam) noexcept
{
	if (to_be_played && !moved)
	{
		data->ClearSelection();
		last_selected = 0;
		data->played = marked;
		PostMessage(parent->Hwnd(), WM_COMMAND, 0, ET_PLAYLIST_DISPLAY);
	}

	to_be_played = false;
	clicked = false;
	moved = false;
	SmartScrolling.Finish();
}
void PLAYLIST_DISPLAY::MouseWheel(WPARAM wParam, LPARAM lParam) noexcept
{
	if (GET_WHEEL_DELTA_WPARAM(wParam) >= 120)
		LineUp();
	else LineDown();

	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	ScreenToClient(parent->Hwnd(), &pt);
	float px = static_cast<float>(pt.x);
	float py = static_cast<float>(pt.y);
	MouseMove(px, py, wParam);

	RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}
void PLAYLIST_DISPLAY::Timer(WPARAM wParam) noexcept
{
	if (SmartScrolling.Perform(wParam))
	{
		UpdateShift();
		RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
	}
}
void PLAYLIST_DISPLAY::Command(WPARAM wParam, LPARAM lParam) noexcept
{
	if (ContextMenu.Command(wParam, lParam))
	{
		data->played = last_selected;
		data->ClearSelection();
		last_selected = 0;
		PostMessage(parent->Hwnd(), WM_COMMAND, 0, ET_PLAYLIST_DISPLAY);
	}
}
bool PLAYLIST_DISPLAY::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (hwnd != parent->Hwnd())
		return false;

	float px = static_cast<float>(GET_X_LPARAM(lParam));
	float py = static_cast<float>(GET_Y_LPARAM(lParam));

	switch(uMsg)
	{
		case WM_LBUTTONDOWN: LButtonDown(px, py, wParam); break;
		case WM_RBUTTONDOWN: RButtonDown(px, py, wParam, lParam); break;
		case WM_MOUSEMOVE: MouseMove(px, py, wParam); break;
		case WM_LBUTTONUP: LButtonUp(px, py, wParam); break;
		case WM_MOUSELEAVE: marked = 0; RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT); break;
		case WM_SIZE: UpdateShift(); break;
		case WM_MOUSEWHEEL: MouseWheel(wParam, lParam); break;
		case WM_TIMER: Timer(wParam); break;
		case WM_COMMAND: Command(wParam, lParam); break;
	}

	return true;
}