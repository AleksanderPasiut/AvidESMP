#include <vector>
#include <windowsx.h>

#include "wheel.h"

const float PI = 3.141529f;

WHEEL::WHEEL(WINDOW* parent, const D2D1_POINT_2F& center, float radius) : 
	parent(parent),
	state(ES_NONE),
	center(center),
	radius(radius),
	pos(1),
	stroke(2.5f),
	m1(radius),
	m2(0.85f*radius),
	m3(0.7f*radius),
	m4(0.25f*radius),
	ma(11),
	alpha(0.5f),
	phi(PI/2 - (ma-1)*alpha/2)
{}

bool WHEEL::In(float px, float py) const noexcept
{
	float sqdfc = (px-center.x)*(px-center.x)+(py-center.y)*(py-center.y);

	return (m4*m4 <= sqdfc && sqdfc <= m3*m3);
}
void WHEEL::Paint() const noexcept
{
	using namespace D2D1;
	
	for (unsigned i = 0; i < ma; i++)
		parent->Target()->DrawLine(Point2F(center.x+m1*cos(alpha*i+phi), center.y-m1*sin(alpha*i+phi)),
								   Point2F(center.x+m2*cos(alpha*i+phi), center.y-m2*sin(alpha*i+phi)),
								   parent->Brush(WB_MAIN), stroke);
	switch(state)
	{
		case ES_CLICK: parent->Target()->DrawEllipse(Ellipse(center, (m3+m4)/2, (m3+m4)/2), parent->Brush(WB_CLICK), m4-m3); break;
		case ES_HOVER: parent->Target()->DrawEllipse(Ellipse(center, (m3+m4)/2, (m3+m4)/2), parent->Brush(WB_HOVER), m4-m3); break;
	}

	parent->Target()->DrawEllipse(Ellipse(center, m3, m3), parent->Brush(WB_MAIN), stroke);


	parent->Target()->DrawLine(Point2F(center.x+m3*cos(alpha*(ma-1)*(1-pos)+phi),
									   center.y-m3*sin(alpha*(ma-1)*(1-pos)+phi)),
							   Point2F(center.x+m4*cos(alpha*(ma-1)*(1-pos)+phi),
									   center.y-m4*sin(alpha*(ma-1)*(1-pos)+phi)),
							   parent->Brush(WB_MAIN), stroke);

	parent->Target()->DrawEllipse(Ellipse(center, m4, m4), parent->Brush(WB_MAIN), stroke);
}
bool WHEEL::Mouse(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
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
				start = { px-center.x, py-center.y };

				state = ES_CLICK;
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
				ret = true;
			}
			break;
		}
		case WM_MOUSEMOVE:
		{
			if (In(px, py))
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
				float npx = px-center.x;  // punkt klikniêty
				float npy = py-center.y;
				float np = npx*npx+npy*npy;

				if (np < m4*m4)  // zabezpieczenie przed obracaniem zbyt bliskim do osi
					break;

				float opx = -start.y;   // obracam "stary" punkt o 90 stopni przeciwnie 
				float opy = start.x;	// do ruchu wskazówek zegara
				float op = opx*opx+opy*opy;

				if (op < m4*m4)
					break;
				
				float on = (npx-opx)*(npx-opx)+(npy-opy)*(npy-opy);
				float difference = acos((op+np-on)/(2*sqrt(np*op)))-PI/2;

				pos -= difference/4.8f;
				start = { npx, npy };
				if (pos < 0)
					pos = 0;
				if (pos > 1)
					pos = 1;

				PostMessage(parent->Hwnd(), WM_COMMAND, 0, ET_WHEEL);
				RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
				ret = true;
			}

			break;
		}
		case WM_LBUTTONUP:
		{
			if (In(px, py))
			{
				switch(state)
				{
					case ES_CLICK:
					{
						state = ES_HOVER;
						RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
						ret = true;
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

	return false;
}

void WHEEL::SetPos(float new_pos) noexcept
{
	bool redraw = pos == new_pos;

	if (0 <= new_pos && new_pos <= 1)
	{
		pos = new_pos;

		if (redraw)
			RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
	}
}