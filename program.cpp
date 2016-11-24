#include "program.h"

HANDLE PROGRAM::hEvent = 0;

bool PROGRAM::PreOpen() noexcept
{
	hEvent = CreateEventW(0, false, true, L"Avid ESMP Event");

	if (!GetLastError())
		return false;

	wchar_t* command_line = GetCommandLineW();
	int argc;
	CommandLineToArgvW(command_line, &argc);

	if (argc < 2)
		return true;

	HWND target = FindWindowW(L"AvidESMP_ListWindowClass", L"Playlist");
	int command_line_length = int(wcslen(command_line));
	HANDLE hMapFile = CreateFileMappingW(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, command_line_length*sizeof(wchar_t), L"AvidESMPFileShared");
	
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;

	void* buffer = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, command_line_length*sizeof(wchar_t));
	memcpy(buffer, command_line, command_line_length*sizeof(wchar_t));
	SendMessage(target, PLAYLIST::WM_TRANSFER, static_cast<WPARAM>(command_line_length), 0);

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);
	return true;
}

PROGRAM::PROGRAM(WNDPROC proc) : 
	Player(),
	Playlist(),
	Controls(proc, &Player, &Playlist),
	Settings(Controls.RetMainHwnd(), &Playlist, &Controls)
{
	Playlist.SetMainHwnd(Controls.MainWindow->Hwnd());
	TRACK_DATA* first_added = Playlist.LoadFilesFromCommandLine();
	Controls.AutoPlay(first_added);
	Controls.ShowWindows();
}
PROGRAM::~PROGRAM() noexcept
{

}

LRESULT PROGRAM::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	Controls.EventProc(hwnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
		case WM_COMMAND:
		{
			if (!wParam && lParam == ET_SETTINGS)
				Settings.Proceed();

			break;
		}
		case PLAYLIST::WM_TRANSFER:
		{
			TRACK_DATA* first_added = Playlist.Transfer(wParam);
			Controls.AutoPlay(first_added);
			RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			break;
		}
		case PLAYLIST::WM_DROPFILESLOADED:
		{
			POINT pt;
			DragQueryPoint(reinterpret_cast<DND_DATA*>(wParam)->hDrop, &pt);
			auto it = Controls.RetIteratorFromPoint(&pt);
			Playlist.FinishDragNDrop(wParam, reinterpret_cast<LPARAM>(&it));
			RedrawWindow(hwnd, 0, 0, RDW_INTERNALPAINT);
			break;
		}
	}

	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
