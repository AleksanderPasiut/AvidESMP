#include "playlist_smart_scrolling.h"

PLAYLIST_SMART_SCROLLING::PLAYLIST_SMART_SCROLLING(HWND hwnd, float& shift, const D2D1_RECT_F& rect) : 
	hwnd(hwnd), shift(shift), rect(rect), ID(0x100), elapse(50), speed(0), margin(20), ref_speed(5) {}

void PLAYLIST_SMART_SCROLLING::Set() noexcept
{
	speed = 0;
	SetTimer(hwnd, ID, elapse, 0);
}
void PLAYLIST_SMART_SCROLLING::Update(float px, float py) noexcept
{
	if (py <= rect.top + margin)
		speed = ((py-rect.top)/margin-1)*ref_speed;
	else if (py >= rect.bottom-margin)
		speed = ((py-rect.bottom)/margin+1)*ref_speed;
}
bool PLAYLIST_SMART_SCROLLING::Perform(WPARAM wParam) noexcept
{
	if (wParam == ID)
	{
		shift += speed;
		return true;
	}
	else return false;
}
void PLAYLIST_SMART_SCROLLING::Finish() noexcept
{
	speed = 0;
	KillTimer(hwnd, ID);
}