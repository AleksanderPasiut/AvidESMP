#include "graphics.h"

GRAPHICS::GRAPHICS()
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2d1Factory)))
		throw 0;

	try
	{
		if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&dwriteFactory))))
			throw 0;
	}
	catch(...) { d2d1Factory->Release(); throw; }
}
GRAPHICS::~GRAPHICS() noexcept
{
	dwriteFactory->Release();
	d2d1Factory->Release();
}