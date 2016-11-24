#pragma once

#include <string>
#include "BASS\c\bass.h"

#pragma comment(lib,"BASS\\c\\x64\\bass.lib")

enum TRACK_PLAYER_TYPE
{
	TPT_NONE,
	TPT_BASS
};
enum TRACK_TYPE
{
	TT_UNKNOWN,
	TT_OGG,
	TT_MP1,
	TT_MP2,
	TT_MP3,
	TT_AIFF,
	TT_CA,
	TT_WMA,
	TT_WAV_Int,
	TT_WAV_Float
};
union TRACK_HANDLE
{
	HSTREAM hstream;
};

const UINT WM_TRACKEND = 0x8002;

struct TRACK_DATA
{
	static HWND hwnd;

	std::wstring url_file;
	std::wstring url_folder;
	std::wstring name;

	TRACK_PLAYER_TYPE player_type;
	TRACK_TYPE type;
	TRACK_HANDLE handle;
	
	double bitrate;
	double duration;

	bool selected;

	void GetInfoFromUrl() noexcept;

	void BASS_Load();
	void BASS_RetrieveFileType() noexcept;
	void BASS_RetrieveDuration() noexcept;
	void BASS_RetrieveBitrate() noexcept;

	TRACK_DATA(std::wstring& url);
	TRACK_DATA(const TRACK_DATA&) = delete;
	TRACK_DATA& operator= (const TRACK_DATA&) = delete;
	~TRACK_DATA() noexcept;

	friend class PLAYLIST_DATA;
	friend class PLAYLIST;
};

void CALLBACK TrackEnd(HANDLE handle, DWORD channel, DWORD data, void* user);