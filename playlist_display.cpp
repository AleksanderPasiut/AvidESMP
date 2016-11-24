#include "playlist_display.h"

PLAYLIST_DISPLAY::PLAYLIST_DISPLAY(WINDOW* parent, PLAYLIST_DATA* data) : 
	parent(parent),
	data(data),
	clicked(false),
	marked(0),
	last_selected(0),
	to_be_played(false),
	moved(false),
	shift(0),
	m(3),
	h(16),
	SmartScrolling(parent->Hwnd(), shift, rect),
	ContextMenu(parent->Hwnd(), data)
{
	if (FAILED(parent->DWriteFactory()->CreateTextFormat(L"Courier New", NULL,
														 DWRITE_FONT_WEIGHT_NORMAL,
														 DWRITE_FONT_STYLE_NORMAL,
														 DWRITE_FONT_STRETCH_NORMAL,
														 12.0f, L"pl-pl", &format)))
		throw 0;

	try
	{
		if (FAILED(parent->Target()->CreateLayer(&layer)))
			throw 0;

		try
		{
			if (FAILED(parent->D2D1Factory()->CreateRectangleGeometry(D2D1::RectF(0, 0, 1, 1), &mask)))
				throw 0;
		}
		catch(...) { layer->Release(); throw; }
	}
	catch(...) { format->Release(); throw; }

	format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	format->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	DWRITE_TRIMMING trimming = { DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0, };
	format->SetTrimming(&trimming, 0);
}
PLAYLIST_DISPLAY::~PLAYLIST_DISPLAY() noexcept
{
	mask->Release();
	layer->Release();
	format->Release();
}

void PLAYLIST_DISPLAY::SetRect(const D2D1_RECT_F& rect) noexcept
{
	this->rect = rect;
	RedrawWindow(parent->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}
void PLAYLIST_DISPLAY::UpdateShift() noexcept
{
	float lines_amount = static_cast<float>(data->tab.size());
	float des_disp_height = lines_amount*h + (lines_amount+1)*m;

	if (shift > des_disp_height - (rect.bottom-rect.top))
		shift = des_disp_height - (rect.bottom-rect.top);

	if (shift < 0)
		shift = 0;
}
void PLAYLIST_DISPLAY::LineUp() noexcept
{
	shift -= m+h;

	UpdateShift();
}
void PLAYLIST_DISPLAY::LineDown() noexcept
{
	shift += m+h;

	UpdateShift();
}
