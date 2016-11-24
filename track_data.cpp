#include "track_data.h"

HWND TRACK_DATA::hwnd = 0;

void TRACK_DATA::GetInfoFromUrl() noexcept
{
	url_folder = url_file;
	name = url_file;
	for (auto it = url_folder.end()-1, jt = name.end()-1; it != url_folder.begin(); it--, jt--)
		if (*it == L'\\')
		{
			*(++it) = L'*';
			*(++it) = 0;
			url_folder.erase(++it, url_folder.end());

			name.erase(name.begin(), ++jt);
			return;
		}
}

TRACK_DATA::TRACK_DATA(std::wstring& url) : 
	player_type(TPT_NONE),
	type(TT_UNKNOWN),
	bitrate(0),
	duration(0),
	selected(false)
{
	this->url_file = url;
	GetInfoFromUrl();

	try
	{
		BASS_Load();
	}
	catch(...)
	{
		throw;
	}
}
TRACK_DATA::~TRACK_DATA() noexcept
{
	switch(player_type)
	{
		case TPT_BASS: BASS_StreamFree(handle.hstream); break;
	}
}

void CALLBACK TrackEnd(HANDLE handle, DWORD channel, DWORD data, void* user)
{
	PostMessage(TRACK_DATA::hwnd, WM_TRACKEND, 0, 0);
}