#include "window.h"

void WINDOW::InitCursors()
{
	try
	{
		if (!(hcursor[0] = LoadCursor(0, IDC_ARROW)))
			throw 0;

		if (!(hcursor[1] = LoadCursor(0, IDC_SIZEALL)))
			throw 0;

		if (!(hcursor[2] = LoadCursor(0, IDC_SIZENWSE)))
			throw 0;

		if (!(hcursor[3] = LoadCursor(0, IDC_SIZENS)))
			throw 0;

		if (!(hcursor[4] = LoadCursor(0, IDC_SIZENESW)))
			throw 0;

		if (!(hcursor[5] = LoadCursor(0, IDC_SIZEWE)))
			throw 0;
	}
	catch(...)
	{
		FreeCursors();
		throw;
	}
}
void WINDOW::FreeCursors() noexcept
{
	for (unsigned i = 0; i < 6; i++)
		DeleteObject(hcursor[i]);
}
void WINDOW::SetCursor(INTERFACE_MODE im) const noexcept
{
	switch(im)
	{
		case IM_RIM: ::SetCursor(hcursor[1]); break;
		case IM_TOP_LEFT: 
		case IM_BOTTOM_RIGHT: ::SetCursor(hcursor[2]); break;
		case IM_TOP:
		case IM_BOTTOM: ::SetCursor(hcursor[3]); break;
		case IM_TOP_RIGHT:
		case IM_BOTTOM_LEFT: ::SetCursor(hcursor[4]); break;
		case IM_LEFT:
		case IM_RIGHT: ::SetCursor(hcursor[5]); break;
		default: ::SetCursor(hcursor[0]); break;
	}
}