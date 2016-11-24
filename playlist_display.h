#pragma once

#include "window.h"
#include "playlist_data.h"
#include "playlist_smart_scrolling.h"
#include "playlist_context_menu.h"

class PLAYLIST_DISPLAY
{
	WINDOW* parent;
	D2D1_RECT_F rect;
	PLAYLIST_DATA* data;
	IDWriteTextFormat* format;
	ID2D1Layer* layer;
	ID2D1RectangleGeometry* mask;

	bool clicked;
	TRACK_DATA* marked;
	TRACK_DATA* last_selected;
	bool to_be_played;
	bool moved;

	float shift;
	const float m; // margines
	const float h; // wysokoœæ linii

	PLAYLIST_SMART_SCROLLING SmartScrolling;
	PLAYLIST_CONTEXT_MENU ContextMenu;
	
	// playlist_display.cpp
	PLAYLIST_DISPLAY(WINDOW* parent, PLAYLIST_DATA* data);
	PLAYLIST_DISPLAY(const PLAYLIST_DISPLAY&) = delete;
	PLAYLIST_DISPLAY& operator= (const PLAYLIST_DISPLAY&) = delete;
	~PLAYLIST_DISPLAY() noexcept;

	void SetRect(const D2D1_RECT_F& rect) noexcept;
	void UpdateShift() noexcept;
	void LineUp() noexcept;
	void LineDown() noexcept;

	// playlist_display_painting.cpp
	std::wstring RetStringFromType(TRACK_TYPE type) const noexcept;
	std::wstring RetStringFromBitrate(double bitrate) const noexcept;
	std::wstring RetStringFromDuration(double duration) const noexcept;
	void SetLayer() const noexcept;
	void Paint() const noexcept;

	// playlist_display_event_proc.cpp
	TRACK_DATA* RetTrackFromPointHard(float px, float py) const noexcept;
	TRACK_DATA* RetTrackFromPointSoft(float px, float py) const noexcept;
	std::vector<TRACK_DATA*>::iterator RetIteratorFromPoint(const POINT* pt) const noexcept;
	void PerformMoving(float px, float py) noexcept;
	void PerformSelecting(float px, float py) noexcept;
	void LButtonDown(float px, float py, WPARAM wParam) noexcept;
	void RButtonDown(float px, float py, WPARAM wParam, LPARAM lParam) noexcept;
	void MouseMove(float px, float py, WPARAM wParam) noexcept;
	void LButtonUp(float px, float py, WPARAM wParam) noexcept;
	void MouseWheel(WPARAM wParam, LPARAM lParam) noexcept;
	void Timer(WPARAM wParam) noexcept;
	void Command(WPARAM wParam, LPARAM lParam) noexcept;
	bool EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;

	friend class VSLIDER;
	friend class CONTROLS;
};
