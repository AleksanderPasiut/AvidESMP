#include "window.h"

D2D1_COLOR_F CL(const D2D1_COLOR_F& color1, float scale, const D2D1_COLOR_F& color2, float alpha = 1.0f) noexcept
{
	return D2D1::ColorF(color1.r*scale+color2.r*(1-scale),
						color1.g*scale+color2.g*(1-scale),
						color1.b*scale+color2.b*(1-scale), alpha);
}


void WINDOW::InitInternalPainting()
{
	rim = 1.5f;
	margin = 10.0f;
	shw = 50.0f;

	background = D2D1::ColorF(0.08f, 0.15f, 0.08f);
	try
	{
		D2D1_COLOR_F core = D2D1::ColorF(0.1f, 0.8f, 0.1f);

		if (FAILED(target->CreateSolidColorBrush(CL(core, 1.0f, background), &brush[WB_MAIN])))
			throw 0;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.52f, background), &brush[WB_CLICK])))
			throw 1;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.25f, background), &brush[WB_HOVER])))
			throw 2;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.07f, background), &brush[WB_SHADOW])))
			throw 3;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.9f, background, 0.5f), &brush[WB_TRANSLUSCENT])))
			throw 4;
	}
	catch(int error)
	{
		switch(error)
		{
			case 4: brush[WB_SHADOW]->Release();
			case 3: brush[WB_HOVER]->Release();
			case 2: brush[WB_CLICK]->Release();
			case 1: brush[WB_MAIN]->Release();
		}
		throw 0;
	}
}
void WINDOW::UpdateInternalPainting(D2D1_COLOR_F core, D2D1_COLOR_F back) noexcept
{
	ID2D1SolidColorBrush* new_brush[5];

	try
	{
		if (FAILED(target->CreateSolidColorBrush(CL(core, 1.0f, back), &new_brush[WB_MAIN])))
			throw 0;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.52f, back), &new_brush[WB_CLICK])))
			throw 1;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.25f, back), &new_brush[WB_HOVER])))
			throw 2;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.07f, back), &new_brush[WB_SHADOW])))
			throw 3;
		if (FAILED(target->CreateSolidColorBrush(CL(core, 0.6f, background, 0.3f), &new_brush[WB_TRANSLUSCENT])))
			throw 4;
	}
	catch(int error)
	{
		switch(error)
		{
			case 4: new_brush[WB_TRANSLUSCENT]->Release();
			case 3: new_brush[WB_HOVER]->Release();
			case 2: new_brush[WB_CLICK]->Release();
			case 1: new_brush[WB_MAIN]->Release();
		}
	}

	background = back;

	for (unsigned i = 0; i < 5; i++)
	{
		brush[i]->Release();
		brush[i] = new_brush[i];
	}

	RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
}
void WINDOW::FreeInternalPainting() noexcept
{
	for (unsigned i = 0; i < 5; i++)
		brush[i]->Release();
}
void WINDOW::InternalPainting() noexcept
{
	using namespace D2D1;

	D2D1_SIZE_F size = target->GetSize();

	target->DrawRectangle(RectF(margin/2, margin/2, size.width-margin/2, size.height-margin/2), brush[interface_mode == IM_RIM ? (click ? WB_CLICK : WB_HOVER) : WB_SHADOW], margin);

	ID2D1SolidColorBrush* br = brush[click ? WB_CLICK : WB_HOVER];
	switch(interface_mode)
	{
		case IM_TOP_LEFT: target->FillRectangle(RectF(0, 0, margin, margin), br); break;
		case IM_TOP: target->FillRectangle(RectF((size.width-shw)/2, 0, (size.width+shw)/2, margin), br); break;
		case IM_TOP_RIGHT: target->FillRectangle(RectF(size.width-margin, 0, size.width, margin), br); break;
		case IM_LEFT: target->FillRectangle(RectF(0, (size.height-shw)/2, margin, (size.height+shw)/2), br); break;
		case IM_RIGHT: target->FillRectangle(RectF(size.width-margin, (size.height-shw)/2, size.width, (size.height+shw)/2), br); break;
		case IM_BOTTOM_LEFT: target->FillRectangle(RectF(0, size.height-margin, margin, size.height), br); break;
		case IM_BOTTOM: target->FillRectangle(RectF((size.width-shw)/2, size.height-margin, (size.width+shw)/2, size.height), br); break;
		case IM_BOTTOM_RIGHT: target->FillRectangle(RectF(size.width-margin, size.height-margin, size.width, size.height), br); break;
	}

	target->DrawRectangle(RectF(0, 0, size.width, size.height), brush[WB_MAIN], rim);
}
void WINDOW::InitPaint() noexcept
{
	target->BeginDraw();
	target->Clear(background);
}
void WINDOW::EndPaint() noexcept
{
	InternalPainting();
	target->EndDraw();
}