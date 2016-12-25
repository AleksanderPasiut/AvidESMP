#include "controls.h"

CONTROLS::CONTROLS(WNDPROC proc, PLAYER* Player, PLAYLIST* Playlist) : 
	MainWindow(0),
	ListWindow(0),
	ButtonClose(0),
	ButtonMinimize(0),
	ButtonSettings(0),
	ButtonPlaylist(0),
	ButtonPrevious(0),
	ButtonStop(0),
	ButtonPlay(0),
	ButtonPause(0),
	ButtonNext(0),
	Clock(0),
	Wheel(0),
	HSlider(0),
	PlaylistDisplay(0),
	DndLoading(0),
	VSlider(0),
	ButtonLineUp(0),
	ButtonLineDown(0),
	Player(Player),
	Playlist(Playlist),
	autoplay_on_direct_file_open(true),
	repeat(false),
	show_list_window(false)
{
	using namespace D2D1;
	WINDOW::Graphics = &Graphics;

	try
	{
		const D2D1_SIZE_U mwds = { 390, 170 };
		const D2D1_SIZE_U lwds = { 600, 500 };

		MainWindow = new WINDOW(proc, 0, L"AvidESMP_MainWindowClass", L"Avid ESMP", 600, 200, mwds.width, mwds.height);
		MainWindow->SetResizable(false);
		ListWindow = new WINDOW(proc, WS_EX_ACCEPTFILES, L"AvidESMP_ListWindowClass", L"Playlist", 10, 10, lwds.width, lwds.height, MainWindow->hwnd);

		const float mbs = 25;
		const float mx = 240;
		const float my = 20;
		const float mdx = 35;
		
		ButtonClose = new BUTTON_CLOSE(MainWindow, RectF(mx+3*mdx, my, mx+3*mdx+mbs, my+mbs));
		ButtonMinimize = new BUTTON_MINIMIZE(MainWindow, RectF(mx+2*mdx, my, mx+2*mdx+mbs, my+mbs));
		ButtonSettings = new BUTTON_SETTINGS(MainWindow, RectF(mx+mdx, my, mx+mdx+mbs, my+mbs));
		ButtonPlaylist = new BUTTON_PLAYLIST(MainWindow, RectF(mx, my, mx+mbs, my+mbs));

		const float cbs = 30;
		const float cx = 28;
		const float cy = 110;
		const float cdx = 40;

		ButtonPrevious = new BUTTON_PREVIOUS(MainWindow, RectF(cx, cy, cx+cbs, cy+cbs));
		ButtonStop = new BUTTON_STOP(MainWindow, RectF(cx+cdx, cy, cx+cdx+cbs, cy+cbs));
		ButtonPlay = new BUTTON_PLAY(MainWindow, RectF(cx+2*cdx, cy, cx+2*cdx+cbs, cy+cbs));
		ButtonPause = new BUTTON_PAUSE(MainWindow, RectF(cx+3*cdx, cy, cx+3*cdx+cbs, cy+cbs));
		ButtonNext = new BUTTON_NEXT(MainWindow, RectF(cx+4*cdx, cy, cx+4*cdx+cbs, cy+cbs));

		Clock = new CLOCK(MainWindow, RectF(cx, 30, cx+4*cdx+cbs, 75));
		Wheel = new WHEEL(MainWindow, Point2F(304, 100), 40);
		HSlider = new HSLIDER(MainWindow, RectF(cx, 87, cx+4*cdx+cbs, 98), 20);

		PlaylistDisplay = new PLAYLIST_DISPLAY(ListWindow, &Playlist->data);
		DndLoading = new DND_LOADING(ListWindow, PlaylistDisplay);
		VSlider = new VSLIDER(ListWindow, PlaylistDisplay);
		ButtonLineUp = new BUTTON_UP(ListWindow);
		ButtonLineDown = new BUTTON_DOWN(ListWindow);

		PlaceListWindowElements();
	}
	catch(...)
	{
		delete ButtonLineDown;
		delete ButtonLineUp;
		delete VSlider;
		delete DndLoading;
		delete PlaylistDisplay;

		delete HSlider;
		delete Wheel;
		delete Clock;

		delete ButtonNext;
		delete ButtonPause;
		delete ButtonPlay;
		delete ButtonStop;
		delete ButtonPrevious;

		delete ButtonPlaylist;
		delete ButtonSettings;
		delete ButtonMinimize;
		delete ButtonClose;

		delete MainWindow;
		delete ListWindow;
		throw;
	}
}
CONTROLS::~CONTROLS() noexcept
{
	SetMainTimer(false);

	delete ButtonLineDown;
	delete ButtonLineUp;
	delete VSlider;
	delete DndLoading;
	delete PlaylistDisplay;
	delete HSlider;
	delete Wheel;
	delete Clock;
	delete ButtonNext;
	delete ButtonPause;
	delete ButtonPlay;
	delete ButtonStop;
	delete ButtonPrevious;
	delete ButtonPlaylist;
	delete ButtonSettings;
	delete ButtonMinimize;
	delete ButtonClose;
	delete ListWindow;
	delete MainWindow;
}

void CONTROLS::ShowWindows() const noexcept
{
	ShowWindow(MainWindow->hwnd, SW_SHOWNORMAL);
	ShowWindow(ListWindow->hwnd, show_list_window ? SW_SHOWNORMAL : SW_HIDE);
}
void CONTROLS::AutoPlay(TRACK_DATA* arg) noexcept
{
	if (arg && autoplay_on_direct_file_open)
	{
		Playlist->SetPlayed(arg);
		Player->SetPlayed(Playlist->RetPlayedHandle());
		Player->Play();
		SetMainTimer(true);
		Clock->Pause(false);
		HSlider->Activate(true);
	}
}
void CONTROLS::PlaceListWindowElements() const noexcept
{
	using namespace D2D1;

	const D2D1_SIZE_F size = ListWindow->target->GetSize();

	float m = 20;
	float sw = 18;
	float s = 10;
	float bs = 30;

	PlaylistDisplay->SetRect(RectF(m, m, size.width-m-sw-s, size.height-m));
	VSlider->SetRect(RectF(size.width-m-sw, m+sw+s, size.width-m, size.height-m-s-sw));
	ButtonLineUp->SetRect(RectF(size.width-m-sw, m, size.width-m, m+sw));
	ButtonLineDown->SetRect(RectF(size.width-m-sw, size.height-m-sw, size.width-m, size.height-m));
}
bool CONTROLS::EventProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
{
	bool ret = false;

	switch(uMsg)
	{
		case WM_COMMAND: ProcessCommand(wParam, lParam); break;
		case WM_PAINT:
		{
			BeginPaint(hwnd, 0);
			if (hwnd == MainWindow->hwnd)
			{
				MainWindow->InitPaint();
				ButtonClose->Paint();
				ButtonSettings->Paint();
				ButtonMinimize->Paint();
				ButtonPlaylist->Paint();
				ButtonPrevious->Paint();
				ButtonStop->Paint();
				ButtonPlay->Paint();
				ButtonPause->Paint();
				ButtonNext->Paint();
				Clock->Paint();
				Wheel->Paint();
				HSlider->Paint();
				MainWindow->EndPaint();
			}
			else if (hwnd == ListWindow->hwnd)
			{
				ListWindow->InitPaint();
				PlaylistDisplay->Paint();
				DndLoading->Paint();
				VSlider->Paint();
				ButtonLineUp->Paint();
				ButtonLineDown->Paint();
				ListWindow->EndPaint();
			}
			EndPaint(hwnd, 0);
			break;
		}
		case WM_SIZE:
		{
			if (hwnd == ListWindow->hwnd)
				PlaceListWindowElements();

			break;
		}
		case WM_TIMER:
		{
			ProcessTimer(wParam);
			Clock->Timer(wParam);
			DndLoading->Timer(wParam);
			break;
		}
		case WM_TRACKEND: ProcessTrackEnd(); break;
		case WM_DROPFILES: { Playlist->StartDragNDrop(reinterpret_cast<HDROP>(wParam)); break; }
		case PLAYLIST::WM_LOADING_FACTOR:
		{
			if (wParam)
				DndLoading->IncFactor();
			else DndLoading->DecFactor();
		}
	}

	MainWindow->EventProc(hwnd, uMsg, wParam, lParam);
	ListWindow->EventProc(hwnd, uMsg, wParam, lParam);
	
	ret |= ButtonClose->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonMinimize->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonSettings->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonPlaylist->Mouse(hwnd, uMsg, wParam, lParam);

	ret |= ButtonPrevious->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonStop->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonPlay->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonPause->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonNext->Mouse(hwnd, uMsg, wParam, lParam);

	ret |= Clock->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= Wheel->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= HSlider->Mouse(hwnd, uMsg, wParam, lParam);

	ret |= VSlider->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonLineUp->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= ButtonLineDown->Mouse(hwnd, uMsg, wParam, lParam);
	ret |= PlaylistDisplay->EventProc(hwnd, uMsg, wParam, lParam);

	return ret;
}
void CONTROLS::ProcessCommand(WPARAM wParam, LPARAM lParam) noexcept
{
	if (wParam)
		return;

	switch(lParam)
	{
		case ET_CLOSE: PostQuitMessage(0); break;
		case ET_MINIMIZE: ShowWindow(MainWindow->hwnd, SW_MINIMIZE); break;
		case ET_PLAYLIST_BUTTON:
		{
			if (IsWindowVisible(ListWindow->hwnd))
				ShowWindow(ListWindow->hwnd, SW_HIDE);
			else ShowWindow(ListWindow->hwnd, SW_SHOWNORMAL);
			break;
		}
		case ET_LINE_UP: PlaylistDisplay->LineUp(); break;
		case ET_LINE_DOWN: PlaylistDisplay->LineDown(); break;
		case ET_PREVIOUS: ProcessPrevious(); break;
		case ET_STOP: ProcessStop(); break;
		case ET_PLAYLIST_DISPLAY: ProcessPlaylistDisplay(); break;
		case ET_PLAY: ProcessPlay(); break;
		case ET_PAUSE: ProcessPause(); break;
		case ET_NEXT: ProcessNext(); break;
		case ET_WHEEL: Player->SetVolume(Wheel->RetPos()); break;
		default: ProcessHSlider(static_cast<E_TYPE>(lParam));
	}
}
void CONTROLS::ProcessPrevious() noexcept
{
	switch(Player->RetState())
	{
		case BASS_ACTIVE_PLAYING:
		{
			Player->Stop();
			Playlist->DecPlayed();
			RedrawWindow(ListWindow->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			Player->SetPlayed(Playlist->RetPlayedHandle());
			Player->Play();
			break;
		}
	}
}
void CONTROLS::ProcessStop() noexcept
{
	Player->Stop();
	SetMainTimer(false);
	HSlider->Activate(false);
	HSlider->SetPos(0);
	Clock->SetValue(0);
	Clock->Pause(false);
	Playlist->NoPlayed();
	RedrawWindow(ListWindow->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}
void CONTROLS::ProcessPlaylistDisplay() noexcept
{
	Player->Stop();
	Player->SetPlayed(Playlist->RetPlayedHandle());
	RedrawWindow(ListWindow->Hwnd(), 0, 0, RDW_INTERNALPAINT);
	Player->Play();
	HSlider->Activate(true);
	SetMainTimer(true);
}
void CONTROLS::ProcessPlay() noexcept
{
	switch(Player->RetState())
	{
		case BASS_ACTIVE_PLAYING:
		{
			if (Playlist->RetFirstSelected())
			{
				Player->Stop();
				Playlist->SetPlayed(Playlist->RetFirstSelected());
				Playlist->ClearSelection();
				Player->SetPlayed(Playlist->RetPlayedHandle());
				RedrawWindow(ListWindow->Hwnd(), 0, 0, RDW_INTERNALPAINT);
				Player->Play();
			}
			break;
		}
		case BASS_ACTIVE_STOPPED: 
		{
			if (Playlist->RetFirstSelected())
			{
				Playlist->SetPlayed(Playlist->RetFirstSelected());
				Playlist->ClearSelection();
			}
			else Playlist->SetFirstPlayed();

			RedrawWindow(ListWindow->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			Player->SetPlayed(Playlist->RetPlayedHandle());
			Player->Play();
			HSlider->Activate(true);
			SetMainTimer(true);
			break;
		}
		case BASS_ACTIVE_PAUSED:
		{
			Player->Resume();
			Clock->Pause(false);
			SetMainTimer(true);
			break;
		}
	}
}
void CONTROLS::ProcessPause() noexcept
{
	Player->Pause();
	Clock->Pause(true);
	SetMainTimer(false);
}
void CONTROLS::ProcessNext() noexcept
{
	switch(Player->RetState())
	{
		case BASS_ACTIVE_PLAYING:
		{
			Player->Stop();
			Playlist->IncPlayed();
			RedrawWindow(ListWindow->Hwnd(), 0, 0, RDW_INTERNALPAINT);
			Player->SetPlayed(Playlist->RetPlayedHandle());
			Player->Play();
			break;
		}
	}
}
void CONTROLS::SetMainTimer(bool arg) noexcept
{
	switch(arg)
	{
		case true: SetTimer(MainWindow->Hwnd(), TIMER_ID, elapse, 0); break;
		case false: KillTimer(MainWindow->Hwnd(), TIMER_ID); break;
	}
}
void CONTROLS::ProcessTimer(WPARAM wParam) noexcept
{
	if (wParam != TIMER_ID)
		return;

	Clock->SetValue(static_cast<__int64>(Player->RetPos()));
	HSlider->SetPos(static_cast<float>(Player->RetPos() / Player->RetDuration()));
}
void CONTROLS::ProcessTrackEnd() noexcept
{
	Playlist->IncPlayed();

	if (repeat || !Playlist->IsFirstTrackPlayed())
	{
		Player->SetPlayed(Playlist->RetPlayedHandle());
		Player->Play();
	}
	else
	{
		Player->Stop();
		SetMainTimer(false);
		HSlider->Activate(false);
		HSlider->SetPos(0);
		Clock->SetValue(0);
		Playlist->NoPlayed();
	}
	RedrawWindow(ListWindow->Hwnd(), 0, 0, RDW_INTERNALPAINT);
}
void CONTROLS::ProcessHSlider(E_TYPE msg) noexcept
{
	switch(msg)
	{
		case ET_HSLIDER_START:
		{
			SetMainTimer(false);
			break;
		}
		case ET_HSLIDER_MOVE:
		{
			Clock->SetValue(static_cast<__int64>(Player->RetDuration()*HSlider->RetPos()));
			break;
		}
		case ET_HSLIDER_STOP:
		{
			Player->SetPos(Player->RetDuration()*HSlider->RetPos());
			SetMainTimer(true);
			break;
		}
	}
}

