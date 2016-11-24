#include <vector>

#include "button.h"

BUTTON_CLOSE::BUTTON_CLOSE(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect) { type = ET_CLOSE; }
BUTTON_MINIMIZE::BUTTON_MINIMIZE(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect) { type = ET_MINIMIZE; }
BUTTON_SETTINGS::BUTTON_SETTINGS(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect) { type = ET_SETTINGS; }
BUTTON_PLAYLIST::BUTTON_PLAYLIST(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect) { type = ET_PLAYLIST_BUTTON; }
BUTTON_PREVIOUS::BUTTON_PREVIOUS(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect)
{
	type = ET_PREVIOUS;

	if (FAILED(parent->D2D1Factory()->CreatePathGeometry(&shape)))
		throw 0;

	try
	{
		ID2D1GeometrySink* sink = 0;

		if (FAILED(shape->Open(&sink)))
			throw 0;

		sink->SetFillMode(D2D1_FILL_MODE_WINDING);
		sink->BeginFigure(D2D1::Point2F(0.3f*rect.left + 0.7f*rect.right,
										0.7f*rect.top + 0.3f*rect.bottom), D2D1_FIGURE_BEGIN_FILLED);
		D2D1_POINT_2F pt[3] = 
		{
			D2D1::Point2F(0.3f*rect.left + 0.7f*rect.right, 0.3f*rect.top + 0.7f*rect.bottom),
			D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right, 0.5f*rect.top + 0.5f*rect.bottom),
			D2D1::Point2F(0.3f*rect.left + 0.7f*rect.right, 0.7f*rect.top + 0.3f*rect.bottom)
		};

		sink->AddLines(pt, 3);
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		sink->Close();
		sink->Release();
	}
	catch(...) { shape->Release(); }
}
BUTTON_PREVIOUS::~BUTTON_PREVIOUS() noexcept { shape->Release(); }
BUTTON_STOP::BUTTON_STOP(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect) { type = ET_STOP; }
BUTTON_PLAY::BUTTON_PLAY(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect)
{
	type = ET_PLAY;

	if (FAILED(parent->D2D1Factory()->CreatePathGeometry(&shape)))
		throw 0;

	try
	{
		ID2D1GeometrySink* sink = 0;

		if (FAILED(shape->Open(&sink)))
			throw 0;

		sink->SetFillMode(D2D1_FILL_MODE_WINDING);
		sink->BeginFigure(D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right,
										0.75f*rect.top + 0.25f*rect.bottom), D2D1_FIGURE_BEGIN_FILLED);
		D2D1_POINT_2F pt[3] = 
		{
			D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right, 0.25f*rect.top + 0.75f*rect.bottom),
			D2D1::Point2F(0.3f*rect.left + 0.7f*rect.right, 0.5f*rect.top + 0.5f*rect.bottom),
			D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right, 0.75f*rect.top + 0.25f*rect.bottom)
		};

		sink->AddLines(pt, 3);
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		sink->Close();
		sink->Release();
	}
	catch(...) { shape->Release(); }
}
BUTTON_PLAY::~BUTTON_PLAY() noexcept { shape->Release(); }
BUTTON_PAUSE::BUTTON_PAUSE(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect) { type = ET_PAUSE; }
BUTTON_NEXT::BUTTON_NEXT(WINDOW* parent, const D2D1_RECT_F& rect) : BUTTON(parent, rect)
{
	type = ET_NEXT;

	if (FAILED(parent->D2D1Factory()->CreatePathGeometry(&shape)))
		throw 0;

	try
	{
		ID2D1GeometrySink* sink = 0;

		if (FAILED(shape->Open(&sink)))
			throw 0;

		sink->SetFillMode(D2D1_FILL_MODE_WINDING);
		sink->BeginFigure(D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right,
										0.7f*rect.top + 0.3f*rect.bottom), D2D1_FIGURE_BEGIN_FILLED);
		D2D1_POINT_2F pt[3] = 
		{
			D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right, 0.3f*rect.top + 0.7f*rect.bottom),
			D2D1::Point2F(0.3f*rect.left + 0.7f*rect.right, 0.5f*rect.top + 0.5f*rect.bottom),
			D2D1::Point2F(0.7f*rect.left + 0.3f*rect.right, 0.7f*rect.top + 0.3f*rect.bottom)
		};

		sink->AddLines(pt, 3);
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		sink->Close();
		sink->Release();
	}
	catch(...) { shape->Release(); }
}
BUTTON_NEXT::~BUTTON_NEXT() noexcept { shape->Release(); }
BUTTON_UP::BUTTON_UP(WINDOW* parent) : BUTTON(parent, D2D1::RectF()) { type = ET_LINE_UP; }
BUTTON_DOWN::BUTTON_DOWN(WINDOW* parent) : BUTTON(parent, D2D1::RectF()) { type = ET_LINE_DOWN; }

void BUTTON_CLOSE::Paint() const noexcept
{
	SharedPainting();

	parent->Target()->DrawLine(D2D1::Point2F(0.2f*rect.right + 0.8f*rect.left,
											 0.2f*rect.bottom + 0.8f*rect.top),
							   D2D1::Point2F(0.8f*rect.right + 0.2f*rect.left,
											 0.8f*rect.bottom + 0.2f*rect.top),
							   parent->Brush(WB_MAIN), 2);

	parent->Target()->DrawLine(D2D1::Point2F(0.2f*rect.right + 0.8f*rect.left,
											 0.8f*rect.bottom + 0.2f*rect.top),
							   D2D1::Point2F(0.8f*rect.right + 0.2f*rect.left,
											 0.2f*rect.bottom + 0.8f*rect.top),
							   parent->Brush(WB_MAIN), 2);
}
void BUTTON_MINIMIZE::Paint() const noexcept
{
	SharedPainting();

	parent->Target()->DrawLine(D2D1::Point2F(0.2f*rect.right + 0.8f*rect.left,
											 0.7f*rect.bottom + 0.3f*rect.top),
							   D2D1::Point2F(0.8f*rect.right + 0.2f*rect.left,
											 0.7f*rect.bottom + 0.3f*rect.top),
							   parent->Brush(WB_MAIN), 2);
}
void BUTTON_SETTINGS::Paint() const noexcept
{
	SharedPainting();

	D2D1_POINT_2F center = D2D1::Point2F((rect.left + rect.right)/2, (rect.top + rect.bottom)/2);
	float r1 = (rect.right - rect.left)*0.18f;
	float r2 = (rect.right - rect.left)*0.33f;
	float stroke = 2.5f;

	parent->Target()->DrawEllipse(D2D1::Ellipse(center, r1, r1), parent->Brush(WB_MAIN), stroke);

	for (float i = 0; i < 6; i+= 1.0f)
	{
		parent->Target()->DrawLine(D2D1::Point2F(center.x + r1*sin(i/3*3.141529f),
												 center.y + r1*cos(i/3*3.141529f)),
								   D2D1::Point2F(center.x + r2*sin(i/3*3.141529f),
												 center.y + r2*cos(i/3*3.141529f)),
									parent->Brush(WB_MAIN), stroke);
	}
}
void BUTTON_PLAYLIST::Paint() const noexcept
{
	SharedPainting();

	float L = rect.left*0.8f + rect.right*0.2f;
	float R = rect.left*0.2f + rect.right*0.8f;
	float T = rect.top*0.75f + rect.bottom*0.25f;
	float dT = (rect.bottom - rect.top)*0.16f;

	for (float i = 0; i < 4; i+= 1.0f)
		parent->Target()->DrawLine(D2D1::Point2F(L, T+i*dT), D2D1::Point2F(R, T+i*dT), parent->Brush(WB_MAIN), 2);
}
void BUTTON_PREVIOUS::Paint() const noexcept
{
	SharedPainting();

	parent->Target()->DrawLine(D2D1::Point2F(rect.left*0.7f + rect.right*0.3f + 1,
											 rect.top*0.7f + rect.bottom*0.3f),
							   D2D1::Point2F(rect.left*0.7f + rect.right*0.3f + 1,
											 rect.top*0.3f + rect.bottom*0.7f), parent->Brush(WB_MAIN), 2);

	parent->Target()->FillGeometry(shape, parent->Brush(WB_MAIN));
}
void BUTTON_STOP::Paint() const noexcept
{
	SharedPainting();

	parent->Target()->FillRectangle(D2D1::RectF(rect.left*0.7f + rect.right*0.3f,
												rect.top*0.7f + rect.bottom*0.3f,
												rect.left*0.3f + rect.right*0.7f,
												rect.top*0.3f + rect.bottom*0.7f), parent->Brush(WB_MAIN));
}
void BUTTON_PLAY::Paint() const noexcept
{
	SharedPainting();

	parent->Target()->FillGeometry(shape, parent->Brush(WB_MAIN));
}
void BUTTON_PAUSE::Paint() const noexcept
{
	SharedPainting();

	parent->Target()->DrawLine(D2D1::Point2F(rect.left*0.7f + rect.right*0.3f + 2,
											 rect.top*0.7f + rect.bottom*0.3f),
							   D2D1::Point2F(rect.left*0.7f + rect.right*0.3f + 2,
											 rect.top*0.3f + rect.bottom*0.7f), parent->Brush(WB_MAIN), 4);

	parent->Target()->DrawLine(D2D1::Point2F(rect.left*0.3f + rect.right*0.7f - 2,
											 rect.top*0.7f + rect.bottom*0.3f),
							   D2D1::Point2F(rect.left*0.3f + rect.right*0.7f - 2,
											 rect.top*0.3f + rect.bottom*0.7f), parent->Brush(WB_MAIN), 4);
}
void BUTTON_NEXT::Paint() const noexcept
{
	SharedPainting();

	parent->Target()->DrawLine(D2D1::Point2F(rect.left*0.3f + rect.right*0.7f - 1,
											 rect.top*0.7f + rect.bottom*0.3f),
							   D2D1::Point2F(rect.left*0.3f + rect.right*0.7f - 1,
											 rect.top*0.3f + rect.bottom*0.7f), parent->Brush(WB_MAIN), 2);

	parent->Target()->FillGeometry(shape, parent->Brush(WB_MAIN));
}
void BUTTON_UP::Paint() const noexcept
{
	using namespace D2D1;

	SharedPainting();

	parent->Target()->DrawLine(Point2F(0.7f*rect.left+0.3f*rect.right,
									   0.3f*rect.top+0.7f*rect.bottom),
							   Point2F(0.5f*rect.left+0.5f*rect.right,
									   0.7f*rect.top+0.3f*rect.bottom), parent->Brush(WB_MAIN), 2);
	parent->Target()->DrawLine(Point2F(0.3f*rect.left+0.7f*rect.right,
									   0.3f*rect.top+0.7f*rect.bottom),
							   Point2F(0.5f*rect.left+0.5f*rect.right,
									   0.7f*rect.top+0.3f*rect.bottom), parent->Brush(WB_MAIN), 2);
}
void BUTTON_DOWN::Paint() const noexcept
{
	using namespace D2D1;

	SharedPainting();

	parent->Target()->DrawLine(Point2F(0.7f*rect.left+0.3f*rect.right,
									   0.7f*rect.top+0.3f*rect.bottom),
							   Point2F(0.5f*rect.left+0.5f*rect.right,
									   0.3f*rect.top+0.7f*rect.bottom), parent->Brush(WB_MAIN), 2);
	parent->Target()->DrawLine(Point2F(0.3f*rect.left+0.7f*rect.right,
									   0.7f*rect.top+0.3f*rect.bottom),
							   Point2F(0.5f*rect.left+0.5f*rect.right,
									   0.3f*rect.top+0.7f*rect.bottom), parent->Brush(WB_MAIN), 2);
}