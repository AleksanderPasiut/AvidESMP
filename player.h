#pragma once

#include <Windows.h>
#include "BASS\c\bass.h"
#include "track_data.h"

class PLAYER
{
	WNDCLASSEXW wc;
	HWND hwnd;

	TRACK_HANDLE* played;

	PLAYER();
	PLAYER(const PLAYER&) = delete;
	PLAYER operator= (const PLAYER&) = delete;
	~PLAYER() noexcept;

	void SetPlayed(TRACK_HANDLE* played) noexcept;
	void Stop() noexcept;
	void Play() noexcept;
	void Pause() noexcept;
	void Resume() noexcept;
	void SetVolume(float) noexcept;
	DWORD RetState() const noexcept;

	void SetPos(double) noexcept;
	double RetPos() const noexcept;
	double RetDuration() const noexcept;

	friend class CONTROLS;
	friend class SETTINGS;
	friend class PROGRAM;
};