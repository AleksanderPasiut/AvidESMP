#include <fstream>
#include "playlist_data.h"

void PLAYLIST_DATA::Load(std::fstream& FS) noexcept
{
	size_t amount;
	FS.read(reinterpret_cast<char*>(&amount), sizeof(size_t));

	bool is_track_played;
	FS.read(reinterpret_cast<char*>(&is_track_played), sizeof(bool));

	int played_pos = -1;
	if (is_track_played)		
		FS.read(reinterpret_cast<char*>(&played_pos), sizeof(int));

	for (size_t i = 0; i < amount; i++)
	{
		size_t url_length;
		FS.read(reinterpret_cast<char*>(&url_length), sizeof(size_t));

		std::wstring url_file(url_length, 0); 
		FS.read(reinterpret_cast<char*>(const_cast<wchar_t*>(url_file.c_str())), url_length*sizeof(wchar_t));

		if (i == played_pos)
		{
			try
			{ 
				played = new TRACK_DATA(url_file);
				tab.push_back(played);
			}
			catch(...) { played_pos = -1; }
		}
		else
		{
			try	{ tab.push_back(new TRACK_DATA(url_file)); }
			catch(...)
			{
				if (i < played_pos)
					played_pos--; 
			}
		}
	}
}
void PLAYLIST_DATA::Save(std::fstream& FS) noexcept
{
	if (clear_playlist_on_shutdown)
		Clear();

	size_t amount = tab.size();
	FS.write(reinterpret_cast<const char*>(&amount), sizeof(size_t));

	bool is_track_played = played != 0;
	FS.write(reinterpret_cast<const char*>(&is_track_played), sizeof(bool));

	if (is_track_played)
	{
		int played_pos = GetTrackPos(played);
		FS.write(reinterpret_cast<const char*>(&played_pos), sizeof(int));
	}

	for (auto it = tab.begin(); it != tab.end(); it++)
	{
		size_t url_length = (*it)->url_file.size()+1;
		FS.write(reinterpret_cast<const char*>(&url_length), sizeof(size_t));

		FS.write(reinterpret_cast<const char*>((*it)->url_file.c_str()), url_length*sizeof(wchar_t));
	}
}