#include "playlist.h"

PLAYLIST::PLAYLIST() : 
	clear_playlist_on_direct_file_open(true),
	clear_playlist_on_shutdown(false)
{
	
}
PLAYLIST::~PLAYLIST() noexcept
{

}

void PLAYLIST::SetMainHwnd(HWND hwnd) const noexcept
{
	TRACK_DATA::hwnd = hwnd;
}
void PLAYLIST::Clear() noexcept
{
	data.Clear();
}
TRACK_DATA* PLAYLIST::LoadFilesFromCommandLine() noexcept
{
	TRACK_DATA* ret = 0;

	int argc = 0;
	wchar_t* command_line = GetCommandLineW();
	wchar_t** argv = CommandLineToArgvW(command_line, &argc);

	if (argc < 2)
		return ret;

	if (clear_playlist_on_direct_file_open)
		data.Clear();

	for (int i = 1; i < argc; i++)
	{
		try
		{
			TRACK_DATA* new_track = new TRACK_DATA(std::wstring(argv[i]));
			data.tab.push_back(new_track);

			if (!ret)
				ret = new_track;
		}
		catch(...) {}
	}
	return ret;
}
TRACK_DATA* PLAYLIST::Transfer(WPARAM wParam) noexcept
{
	HANDLE hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, L"AvidESMPFileShared");

	if (!hMapFile)
		return 0;

	void* buffer = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, static_cast<int>(wParam)*sizeof(wchar_t));

	if (!buffer)
	{
		CloseHandle(hMapFile);
		return 0;
	}

	if (clear_playlist_on_direct_file_open)
		data.Clear();

	TRACK_DATA* ret = 0;

	int argc = 0;
	wchar_t** argv = CommandLineToArgvW(reinterpret_cast<wchar_t*>(buffer), &argc);
	
	for (int i = 1; i < argc; i++)
	{
		try
		{
			TRACK_DATA* new_track = new TRACK_DATA(std::wstring(argv[i]));
			data.tab.push_back(new_track);

			if (!ret)
				ret = new_track;
		}
		catch(...) {}
	}

	UnmapViewOfFile(buffer);
	CloseHandle(hMapFile);

	return ret;
}
void PLAYLIST::SetFirstPlayed() noexcept
{
	if (data.tab.size() > 0)
		data.played = *(data.tab.begin());
}
