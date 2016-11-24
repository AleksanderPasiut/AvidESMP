#include <Windows.h>

#include "program.h"

PROGRAM* Program = 0;

LRESULT CALLBACK WindowEventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (Program)
		return Program->EventProc(hwnd, uMsg, wParam, lParam);
	else return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	if (PROGRAM::PreOpen())
		return 0;

	try	{ Program = new PROGRAM(WindowEventProc); }	catch(...) { return 0; }

	MSG msg;
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	PROGRAM* del = Program;
	Program = 0;
	delete del;

	return static_cast<int>(msg.wParam);
}