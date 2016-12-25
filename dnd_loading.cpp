#include "dnd_loading.h"

 DND_LOADING::DND_LOADING(WINDOW* parent, PLAYLIST_DISPLAY* display) :
	 parent(parent),
	 display(display),
	 factor(0),
	 angle(0)
 {

 }

void DND_LOADING::IncFactor() noexcept
{
	if (!factor)
		SetTimer(parent->Hwnd(), TIMER_ID, elapse, 0);

	factor++;
}
void DND_LOADING::DecFactor() noexcept
{
	factor--;

	if (!factor)
		KillTimer(parent->Hwnd(), TIMER_ID);
}
void DND_LOADING::Timer(WPARAM wParam) noexcept
{
	if (wParam != TIMER_ID)
		return;

	if (factor)
	{
		angle += 0.05f;
		RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
	}
}
void DND_LOADING::Paint() const noexcept
{
	if (!factor)
		return;	

	const float max_diameter = 200;
	const float ss_factor = 0.8f;
	const float diameter = min(max_diameter, ss_factor*min(display->rect.right-display->rect.left, display->rect.bottom-display->rect.top));

	const float d_angle = -0.28f;
	const float el_diameter = 0.1f*diameter;
	const float d_diameter = 0.012f*diameter;

	D2D1_POINT_2F center = { (display->rect.left + display->rect.right)/2, (display->rect.top + display->rect.bottom)/2	};

	for (float diam = el_diameter, ang = angle; diam > 0; diam -= d_diameter, ang += d_angle)
	{
		D2D1_POINT_2F cent = { center.x+diameter*cos(ang)/2, center.y+diameter*sin(ang)/2 };
		parent->Target()->DrawEllipse(D2D1::Ellipse(cent, diam/2, diam/2), parent->Brush(WB_TRANSLUSCENT), 3.0f);
	}
}
