#include <fstream>

#include "settings.h"

SETTINGS* SETTINGS::own = 0;
COLORREF SETTINGS::cust_colors[16];

SETTINGS::SETTINGS(HWND hwnd_parent, PLAYLIST* playlist, CONTROLS* controls) : 
	hwnd_parent(hwnd_parent),
	playlist(playlist),
	controls(controls)
{
	if (own)
		throw 0;
	else own = this;

	InitSavingPath();

	LoadFile();
}
SETTINGS::~SETTINGS() noexcept
{
	SaveFile();
}

void SETTINGS::InitSavingPath() noexcept
{
	wchar_t* tmp = GetCommandLineW();
	int argc;
	wchar_t** argv = CommandLineToArgvW(tmp, &argc);

	path = std::wstring(argv[0]);

	for (auto it = path.end()-1; it != path.begin(); it--)
		if (*it == L'\\')
		{
			path.erase(++it, path.end());
			path += L"config.cfg";
			break;
		}
}

void SETTINGS::RetrieveSet() noexcept
{
	set.interface_color = controls->RetInterfaceColor();
	set.background_color = controls->RetBackgroundColor();
	set.auto_play_on_direct_file_open = controls->RetAutoPlayOnDirectFileOpen();
	set.clear_playlist_on_direct_file_open = playlist->RetClearPlaylistOnDirectFileOpen();
	set.clear_playlist_on_shutdown = playlist->RetClearPlaylistOnShutdown();
	set.repeat = controls->RetRepeat();
}
void SETTINGS::Proceed() noexcept
{
	RetrieveSet();

	if (DialogBox(0, "dialog_settings", hwnd_parent, reinterpret_cast<DLGPROC>(DialogSettingsProc)))
		ApplySet();
}
void SETTINGS::ApplySet() const noexcept
{
	controls->SetRepeat(set.repeat);
	playlist->SetClearPlaylistOnShutdown(set.clear_playlist_on_shutdown);
	playlist->SetClearPlaylistOnDirectFileOpen(set.clear_playlist_on_direct_file_open);
	controls->SetAutoPlayOnDirectFileOpen(set.auto_play_on_direct_file_open);
	controls->SetColors(set.interface_color, set.background_color);
}

void SETTINGS::LoadFile() noexcept
{
	using namespace std;

	fstream FS(path, fstream::in);

	if (!FS)
		return;

	FS.read(reinterpret_cast<char*>(&set), sizeof(SETTINGS_SET));
	ApplySet();

	playlist->Load(FS);
	controls->Load(FS);

	FS.close();
}
void SETTINGS::SaveFile() noexcept
{
	using namespace std;

	fstream FS(path, fstream::out);

	if (!FS)
		return;

	RetrieveSet();
	FS.write(reinterpret_cast<const char*>(&set), sizeof(SETTINGS_SET));

	playlist->Save(FS);
	controls->Save(FS);

	FS.close();
}

void SETTINGS::InitDialogValues(HWND hwnd) noexcept
{
	CheckDlgButton(hwnd, CTRL_CB_AUTO_PLAY_ON_DIRECT_FILE_OPEN, own->set.auto_play_on_direct_file_open);
	CheckDlgButton(hwnd, CTRL_CB_CLEAR_PLAYLIST_ON_DIRECT_FILE_OPEN, own->set.clear_playlist_on_direct_file_open);
	CheckDlgButton(hwnd, CTRL_CB_CLEAR_PLAYLIST_ON_SHUTDOWN, own->set.clear_playlist_on_shutdown);
	CheckDlgButton(hwnd, CTRL_CB_REPEAT, own->set.repeat);
}
void SETTINGS::ChangeAutoplayOnDirectFileOpen(HWND hwnd) noexcept
{
	own->set.auto_play_on_direct_file_open = !own->set.auto_play_on_direct_file_open;
	CheckDlgButton(hwnd, CTRL_CB_AUTO_PLAY_ON_DIRECT_FILE_OPEN, own->set.auto_play_on_direct_file_open);
}
void SETTINGS::ChangeClearPlaylistOnDirectFileOpen(HWND hwnd) noexcept
{
	own->set.clear_playlist_on_direct_file_open = !own->set.clear_playlist_on_direct_file_open;
	CheckDlgButton(hwnd, CTRL_CB_CLEAR_PLAYLIST_ON_DIRECT_FILE_OPEN, own->set.clear_playlist_on_direct_file_open);
}
void SETTINGS::ChangeClearPlaylistOnShutdown(HWND hwnd) noexcept
{
	own->set.clear_playlist_on_shutdown = !own->set.clear_playlist_on_shutdown;
	CheckDlgButton(hwnd, CTRL_CB_CLEAR_PLAYLIST_ON_SHUTDOWN, own->set.clear_playlist_on_shutdown);
}
void SETTINGS::ChangeRepeat(HWND hwnd) noexcept
{
	own->set.repeat = !own->set.repeat;
	CheckDlgButton(hwnd, CTRL_CB_REPEAT, own->set.repeat);
}
void SETTINGS::ChangeColorBackground(HWND hwnd) noexcept
{
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = cust_colors;
	cc.rgbResult = RGB(static_cast<unsigned>(255*own->set.background_color.r),
					   static_cast<unsigned>(255*own->set.background_color.g),
					   static_cast<unsigned>(255*own->set.background_color.b));
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if(ChooseColor(&cc))
	{
		own->set.background_color = D2D1::ColorF(static_cast<float>(GetRValue(cc.rgbResult))/255,
												 static_cast<float>(GetGValue(cc.rgbResult))/255,
												 static_cast<float>(GetBValue(cc.rgbResult))/255);
	}
}
void SETTINGS::ChangeColorInterface(HWND hwnd) noexcept
{
	CHOOSECOLOR cc;
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = cust_colors;
	cc.rgbResult = RGB(static_cast<unsigned>(255*own->set.interface_color.r),
					   static_cast<unsigned>(255*own->set.interface_color.g),
					   static_cast<unsigned>(255*own->set.interface_color.b));
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if(ChooseColor(&cc))
	{
		own->set.interface_color = D2D1::ColorF(static_cast<float>(GetRValue(cc.rgbResult))/255,
												static_cast<float>(GetGValue(cc.rgbResult))/255,
												static_cast<float>(GetBValue(cc.rgbResult))/255);
	}
}

BOOL CALLBACK DialogSettingsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
		switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			SETTINGS::InitDialogValues(hwnd);
			break;
		}
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CTRL_CB_AUTO_PLAY_ON_DIRECT_FILE_OPEN: SETTINGS::ChangeAutoplayOnDirectFileOpen(hwnd); break;
				case CTRL_CB_CLEAR_PLAYLIST_ON_DIRECT_FILE_OPEN: SETTINGS::ChangeClearPlaylistOnDirectFileOpen(hwnd); break;
				case CTRL_CB_CLEAR_PLAYLIST_ON_SHUTDOWN: SETTINGS::ChangeClearPlaylistOnShutdown(hwnd); break;
				case CTRL_CB_REPEAT: SETTINGS::ChangeRepeat(hwnd); break;
				case CTRL_PB_SET_BACKGROUND_COLOR: SETTINGS::ChangeColorBackground(hwnd); break;
				case CTRL_PB_SET_INTERFACE_COLOR: SETTINGS::ChangeColorInterface(hwnd); break;
				case CTRL_OK: EndDialog(hwnd, 1); return TRUE;
				case CTRL_CANCEL: EndDialog(hwnd, 0); return TRUE;
			}
			break;
		}
	}
	return FALSE;
}