#include "player.h"

PLAYER::PLAYER() : played(0)
{
	ZeroMemory(&wc, sizeof(WNDCLASSEXW));
	wc.cbSize = sizeof(WNDCLASSEXW);
	wc.lpszClassName = L"AvidESMP_BASSWindowClass";
	wc.lpfnWndProc = DefWindowProc;
	wc.hInstance = GetModuleHandle(0);

	if (!RegisterClassExW(&wc))
		throw 0;

	try
	{
		hwnd = CreateWindowExW(0, wc.lpszClassName, L"AvidESMP_BASSWindow", 0, 0, 0, 0, 0, 0, 0, wc.hInstance, 0);

		try
		{
			BASS_Init(-1, 44100, 0, hwnd, 0);
			BASS_SetConfig(BASS_CONFIG_CURVE_VOL, false);
		}
		catch(...) { DestroyWindow(hwnd); throw; }
	}
	catch(...) { UnregisterClassW(wc.lpszClassName, wc.hInstance); throw; }
}
PLAYER::~PLAYER() noexcept
{
	BASS_Free();
	DestroyWindow(hwnd);
	UnregisterClassW(wc.lpszClassName, wc.hInstance);
}

void PLAYER::SetPlayed(TRACK_HANDLE* played) noexcept
{
	this->played = played;
}
void PLAYER::Stop() noexcept
{
	if (played)
	{
		BASS_ChannelStop(played->hstream);
	}
}
void PLAYER::Play() noexcept
{
	if (played)
	{
		BASS_Stop();
		BASS_Start();
		BASS_ChannelPlay(played->hstream, true);
	}
}
void PLAYER::Pause() noexcept
{
	BASS_Pause();
}
void PLAYER::Resume() noexcept
{
	if (played)
	{
		BASS_Start();
		BASS_ChannelPlay(played->hstream, false);
	}
}
void PLAYER::SetVolume(float volume) noexcept
{
	BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, static_cast<DWORD>(10000.0f*volume));
}
DWORD PLAYER::RetState() const noexcept
{
	if (!played)
		return BASS_ACTIVE_STOPPED;
	else return BASS_ChannelIsActive(played->hstream);
}

void PLAYER::SetPos(double pos) noexcept
{
	if (!played || !played->hstream)
		return;

	__int64 qword = BASS_ChannelSeconds2Bytes(played->hstream, pos);
	BASS_ChannelSetPosition(played->hstream, qword, BASS_POS_BYTE);
}
double PLAYER::RetPos() const noexcept
{
	if (!played || !played->hstream)
		return 0;

	__int64 qword = BASS_ChannelGetPosition(played->hstream, BASS_POS_BYTE);
	return BASS_ChannelBytes2Seconds(played->hstream, qword);
}
double PLAYER::RetDuration() const noexcept
{
	if (!played || !played->hstream)
		return 0;

	__int64 qword = BASS_ChannelGetLength(played->hstream, BASS_POS_BYTE);
	return BASS_ChannelBytes2Seconds(played->hstream, qword);
}
