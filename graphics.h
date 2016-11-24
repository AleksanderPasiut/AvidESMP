#pragma once

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

class GRAPHICS
{
public:
	ID2D1Factory* d2d1Factory;
	IDWriteFactory* dwriteFactory;

	GRAPHICS();
	GRAPHICS(const GRAPHICS&) = delete;
	GRAPHICS& operator= (const GRAPHICS&) = delete;
	~GRAPHICS() noexcept;

	friend class MAIN_WINDOW;
	friend class CONTROLS;
};