#include "playlist_display.h"

std::wstring PLAYLIST_DISPLAY::RetStringFromType(TRACK_TYPE type) const noexcept
{
	switch(type)
	{
		case TT_OGG: return std::wstring(L"OGG");
		case TT_MP1: return std::wstring(L"MP1");
		case TT_MP2: return std::wstring(L"MP2");
		case TT_MP3: return std::wstring(L"MP3");
		case TT_AIFF: return std::wstring(L"AIFF");
		case TT_CA: return std::wstring(L"CA");
		case TT_WMA: return std::wstring(L"WMA");
		case TT_WAV_Int: return std::wstring(L"WAV");
		case TT_WAV_Float: return std::wstring(L"WAV");
		default: return std::wstring(L"N/K");
	}
}
std::wstring PLAYLIST_DISPLAY::RetStringFromBitrate(double bitrate) const noexcept
{
	std::wstring ret = std::to_wstring(bitrate);

	for (auto it = ret.begin()+3; it != ret.end(); it++)
		if (*(it-3) == L'.')
		{
			ret.erase(it, ret.end());
			break;
		}

	return ret + L" kb/s";
}
std::wstring PLAYLIST_DISPLAY::RetStringFromDuration(double duration) const noexcept
{
	unsigned hours = static_cast<unsigned>(duration/3.6e3);
	unsigned minutes = static_cast<unsigned>((duration-hours*3.6e3)/60);
	unsigned seconds = static_cast<unsigned>((duration-hours*3.6e3-minutes*60));

	std::wstring ret;

	if (hours)
	{
		ret += std::to_wstring(hours) + L":";

		if (minutes < 10)
			ret += L"0";
	}
	
	ret += std::to_wstring(minutes) + L":";

	if (seconds < 10)
		ret += L"0";

	ret += std::to_wstring(seconds);

	return ret;
}

void PLAYLIST_DISPLAY::SetLayer() const noexcept
{
	using namespace D2D1;
	Matrix3x2F transform = Matrix3x2F::Scale(SizeF(rect.right-rect.left, rect.bottom-rect.top)) * Matrix3x2F::Translation(SizeF(rect.left, rect.top));
	parent->Target()->PushLayer(LayerParameters(InfiniteRect(),
												mask,
												D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
												transform),
								layer);
}
void PLAYLIST_DISPLAY::Paint() const noexcept
{
	using namespace D2D1;

	if (!data)
		return;

	const float stroke = 1.5f;
	const float w_nr = 60;
	const float w_typ = 25;
	const float w_dur = 60;
	const float w_brt = 90;

	SetLayer();

	unsigned i = static_cast<unsigned>(shift/(h+m));
	unsigned limit = min(static_cast<unsigned>(data->tab.size()), static_cast<unsigned>((shift+rect.bottom-rect.top)/(h+m))+1);
	for (auto it = data->tab.begin() + i; i < limit; i++, it++)
	{
		ID2D1SolidColorBrush* brush = parent->Brush(WB_SHADOW);
		if (marked == *it)
			brush = parent->Brush(clicked ? WB_CLICK : WB_HOVER);
		
		parent->Target()->FillRectangle(RectF(rect.left+m, 
											  rect.top+m+i*(h+m)-shift,
											  rect.right-m,
											  rect.top+(i+1)*(h+m)-shift), brush);
		
		D2D1_RECT_F rect_nr = RectF(rect.left+m,
									rect.top+m+i*(h+m)-shift,
									rect.left+m+w_nr,
									rect.top+(i+1)*(h+m)-shift);

		D2D1_RECT_F rect_name = RectF(rect.left+m+w_nr+m,
									  rect.top+m+i*(h+m)-shift,
									  rect.right-m-w_brt-m-w_typ-m-w_dur-m,
									  rect.top+(i+1)*(h+m)-shift);

		D2D1_RECT_F rect_dur = RectF(rect.right-m-w_brt-m-w_typ-m-w_dur,
									 rect.top+m+i*(h+m)-shift,
									 rect.right-m-w_brt-m-w_typ-m,
									 rect.top+(i+1)*(h+m)-shift);

		D2D1_RECT_F rect_typ = RectF(rect.right-m-w_brt-m-w_typ,
									 rect.top+m+i*(h+m)-shift,
									 rect.right-m-w_brt-m,
									 rect.top+(i+1)*(h+m)-shift);

		D2D1_RECT_F rect_brt = RectF(rect.right-m-w_brt,
									 rect.top+m+i*(h+m)-shift,
									 rect.right-m,
									 rect.top+(i+1)*(h+m)-shift);
		
		if (*it == data->played)
		{
			parent->Target()->DrawLine(Point2F(rect.left+2*m, rect.top+2*m+i*(h+m)-shift), Point2F(rect.left+m+h, rect.top+m+i*(h+m)+h/2-shift), parent->Brush(WB_MAIN), stroke);
			parent->Target()->DrawLine(Point2F(rect.left+2*m, rect.top-m+(i+1)*(h+m)-shift), Point2F(rect.left+m+h, rect.top+m+i*(h+m)+h/2-shift), parent->Brush(WB_MAIN), stroke);
		}

		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		parent->Target()->DrawTextA((*it)->name.c_str(), static_cast<UINT32>((*it)->name.length()), format, rect_name, parent->Brush(WB_MAIN));
		std::wstring type = RetStringFromType((*it)->type);
		parent->Target()->DrawTextA(type.c_str(), static_cast<UINT32>(type.length()), format, rect_typ, parent->Brush(WB_MAIN));

		format->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		std::wstring nr = std::to_wstring(i+1);
		parent->Target()->DrawTextA(nr.c_str(), static_cast<UINT32>(nr.length()), format, rect_nr, parent->Brush(WB_MAIN));
		std::wstring dur = RetStringFromDuration((*it)->duration);
		parent->Target()->DrawTextA(dur.c_str(), static_cast<UINT32>(dur.length()), format, rect_dur, parent->Brush(WB_MAIN));
		std::wstring brt = RetStringFromBitrate((*it)->bitrate);
		parent->Target()->DrawTextA(brt.c_str(), static_cast<UINT32>(brt.length()), format, rect_brt, parent->Brush(WB_MAIN));

		if ((*it)->selected)
			parent->Target()->DrawRectangle(RectF(rect.left+m, 
												  rect.top+m+i*(h+m)-shift,
												  rect.right-m,
												  rect.top+(i+1)*(h+m)-shift), parent->Brush(WB_MAIN), 1.0f);
	}

	parent->Target()->PopLayer();
	parent->Target()->DrawRectangle(rect, parent->Brush(WB_MAIN), stroke);
}