#include <fstream>
#include "controls.h"

void CONTROLS::Load(std::fstream& FS) noexcept
{
	RECT rect;

	FS.read(reinterpret_cast<char*>(&rect), sizeof(RECT));
	MainWindow->SetRect(&rect);
	ShowWindow(MainWindow->Hwnd(), SW_SHOWNORMAL);

	FS.read(reinterpret_cast<char*>(&show_list_window), sizeof(bool));

	FS.read(reinterpret_cast<char*>(&rect), sizeof(RECT));
	ListWindow->SetRect(&rect);

	float volume;
	FS.read(reinterpret_cast<char*>(&volume), sizeof(float));
	Wheel->SetPos(volume);
	Player->SetVolume(volume);

	bool played;
	FS.read(reinterpret_cast<char*>(&played), sizeof(bool));

	if (played)
	{
		double pos;
		FS.read(reinterpret_cast<char*>(&pos), sizeof(double));

		Player->SetPlayed(Playlist->RetPlayedHandle());
		double duration = Player->RetDuration();

		HSlider->SetPos(static_cast<float>(pos/duration));
		HSlider->Activate(true);
		Clock->SetValue(static_cast<__int64>(pos));
		Player->SetPos(pos);
		Player->SetVolume(0);
		Player->Resume();
		Player->Pause();
		Player->SetVolume(volume);
		Clock->Pause(true);
		SetMainTimer(false);
	}
}
void CONTROLS::Save(std::fstream& FS) const noexcept
{
	bool visibility;
	RECT rect;
	
	MainWindow->GetRect(&rect);
	FS.write(reinterpret_cast<const char*>(&rect), sizeof(RECT));

	visibility = IsWindowVisible(ListWindow->hwnd) != 0;
	FS.write(reinterpret_cast<const char*>(&visibility), sizeof(bool));

	ListWindow->GetRect(&rect);
	FS.write(reinterpret_cast<const char*>(&rect), sizeof(RECT));

	float volume = Wheel->RetPos();
	FS.write(reinterpret_cast<const char*>(&volume), sizeof(float));

	bool played = Player->RetState() != BASS_ACTIVE_STOPPED;
	FS.write(reinterpret_cast<const char*>(&played), sizeof(bool));

	if (played)
	{
		double pos = Player->RetPos();
		FS.write(reinterpret_cast<const char*>(&pos), sizeof(double));
	}
}