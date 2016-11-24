#include "window.h"

GRAPHICS* WINDOW::Graphics = 0;

WINDOW::WINDOW(WNDPROC proc, DWORD ex_style, const wchar_t* class_name, const wchar_t* window_name, int x, int y, int width, int height, HWND parent)
{
	ZeroMemory(&wc, sizeof(WNDCLASSEXW));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpfnWndProc = proc;
	wc.lpszClassName = class_name;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = LoadIcon(GetModuleHandle(0), "icon");
	wc.hIconSm = LoadIcon(GetModuleHandle(0), "icon_sm");
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

	if (!RegisterClassExW(&wc))
		throw 0;

	try
	{
		hwnd = CreateWindowExW(ex_style, wc.lpszClassName, window_name, WS_POPUP | WS_MINIMIZEBOX, x, y, width, height, parent, 0, wc.hInstance, 0);

		if (!hwnd)
			throw 0;

		try
		{
			HRESULT hresult = Graphics->d2d1Factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
																			D2D1::HwndRenderTargetProperties(hwnd, D2D1::SizeU(width, height)),
																			&target);
			if (FAILED(hresult))
				throw 0;

			try
			{
				InitInternalPainting();
				InitInterface();

				try
				{
					InitCursors();

					tracking = false;
					tme.cbSize = sizeof(TRACKMOUSEEVENT);
					tme.hwndTrack = hwnd;
					tme.dwFlags = TME_LEAVE;
					tme.dwHoverTime = 0;
				}
				catch(...) { FreeInternalPainting(); throw; }
			}
			catch(...) { target->Release(); throw; }
		}
		catch(...) { DestroyWindow(hwnd); throw; }
	}
	catch(...) { UnregisterClassW(wc.lpszClassName, wc.hInstance); throw; }
}
WINDOW::~WINDOW() noexcept
{
	FreeCursors();
	FreeInternalPainting();
	target->Release();
	DestroyWindow(hwnd);
	DeleteObject(wc.hCursor);
	DeleteObject(wc.hIconSm);
	DeleteObject(wc.hIcon);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

void WINDOW::SetRect(const RECT* rect) noexcept
{
	if (resizable)
	{
		target->Resize(D2D1::SizeU(rect->right - rect->left, rect->bottom - rect->top));
		SetWindowPos(hwnd, 0, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_NOZORDER | SWP_NOREDRAW);
		PostMessage(hwnd, WM_SIZE, 0, 0);
	}
	else SetWindowPos(hwnd, 0, rect->left, rect->top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOREDRAW);
}
void WINDOW::GetRect(RECT* rect) const noexcept
{
	GetWindowRect(hwnd, rect);
}
