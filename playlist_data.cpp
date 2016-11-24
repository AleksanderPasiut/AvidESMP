#include "playlist_data.h"

PLAYLIST_DATA::PLAYLIST_DATA() : 
	clear_playlist_on_shutdown(false)
{
	played = 0;
}
PLAYLIST_DATA::~PLAYLIST_DATA() noexcept
{
	Clear();
}

int PLAYLIST_DATA::GetTrackPos(TRACK_DATA* pt) const noexcept
{
	int pt_pos = 0;
	for (auto it = tab.begin(); it != tab.end(); it++)
	{
		if (*it == pt)
			return pt_pos;

		pt_pos++;
	}

	return -1;
}
int PLAYLIST_DATA::Difference(TRACK_DATA* pt1, TRACK_DATA* pt2) const noexcept
{
	if (pt1 == pt2)
		return 0;

	return GetTrackPos(pt1) - GetTrackPos(pt2);
}
void PLAYLIST_DATA::SetSelection(TRACK_DATA* pt1, TRACK_DATA* pt2) const noexcept
{
	if (pt1 == pt2)
	{
		pt1->selected = true;
		return;
	}

	bool fill = false;
	for (auto it = tab.begin(); it != tab.end(); it++)
	{
		if (fill)
		{
			(*it)->selected = true;

			if ((*it) == pt1 || (*it) == pt2)
				break;
		}
		else
		{
			if ((*it) == pt1 || (*it) == pt2)
			{
				fill = true;
				(*it)->selected = true;
			}
		}
	}
}
void PLAYLIST_DATA::SmartSelection(TRACK_DATA* pivot, TRACK_DATA* start, TRACK_DATA* end) const noexcept
{
	if (start == end)
		return;

	int pivot_pos = GetTrackPos(pivot);
	int start_pos = GetTrackPos(start);
	int end_pos = GetTrackPos(end);

	if ((end_pos < start_pos && start_pos <= pivot_pos) ||
		(end_pos > start_pos && start_pos >= pivot_pos))
	{
		end->selected = !end->selected;
	}
	else if ((start_pos < end_pos && end_pos <= pivot_pos) ||
			 (start_pos > end_pos && end_pos >= pivot_pos))
	{
		start->selected = !start->selected;
	}
	else
	{
		end->selected = !end->selected;
		start->selected = !start->selected;
	}

	if (abs(start_pos - end_pos) < 2)
		return;

	bool filling = false;
	TRACK_DATA* limiter = 0;
	for (auto it = tab.begin(); *it != limiter; it++)
	{
		if (filling)
		{
			if (*it != pivot)
				(*it)->selected = !(*it)->selected;
		}
		else if (*it == start)
		{
			limiter = end;
			filling = true;
		}
		else if (*it == end)
		{
			limiter = start;
			filling = true;
		}
	}
}
bool PLAYLIST_DATA::MoveSelection(__int64 interval) noexcept
{
	if (!interval)
		return false;

	// uzyskanie iteratorów do pierwszego i ostatniego zaznaczonego tracka
	auto first_selected = tab.begin();
	for (;first_selected < tab.end(); first_selected++)
		if ((*first_selected)->selected)
			break;

	if (first_selected == tab.end())
		return false;

	auto last_selected = tab.end()-1;
	for (;last_selected >= tab.begin(); last_selected--)
		if ((*last_selected)->selected)
			break;

	// blokada aby nie przesun¹æ zbyt daleko
	if (interval > 0)
	{
		int limit = 0;
		for (auto it = tab.end(); it != last_selected; it--, limit++);
		
		if (interval > limit-1)
			interval = limit-1;
	}
	else
	{
		int limit = 0;
		for (auto it = tab.begin(); it != first_selected; it++, limit++);
		
		if (interval < -limit)
			interval = -limit;
	}

	if (!interval)
		return false;

	try
	{
		std::vector<TRACK_DATA*> backup = tab;

		for (auto it = tab.begin(); it != tab.end(); it++)
			(*it) = 0;

		if (interval > 0)
		{
			for (auto it = backup.begin(), jt = tab.begin()+interval; jt != tab.end(); it++, jt++)
				if ((*it)->selected)
				{
					*jt = *it;
					*it = 0;
				}
		}
		else
		{
			interval = -interval;

			for (auto it = backup.begin()+interval, jt = tab.begin(); it != backup.end(); it++, jt++)
				if ((*it)->selected)
				{
					*jt = *it;
					*it = 0;
				}
		}

		for (auto it = tab.begin(), jt = backup.begin(); it != tab.end() && jt != backup.end(); it++, jt++)
		{
			while(*it)
				it++;

			while (!*jt)
				jt++;

			*it = *jt;
		}
	}
	catch(...) {}

	return true;
}
void PLAYLIST_DATA::ClearSelection() const noexcept
{
	for (auto it = tab.begin(); it != tab.end(); it++)
		(*it)->selected = false;
}
void PLAYLIST_DATA::DeleteSelection() noexcept
{
	for (auto it = tab.begin(); it != tab.end();)
	{
		if ((*it)->selected)
		{
			delete *it;
			it = tab.erase(it);
		}
		else it++;
	}
}
void PLAYLIST_DATA::Clear() noexcept
{
	for (auto it = tab.begin(); it != tab.end(); it++)
		delete *it;

	tab.clear();
}
void PLAYLIST_DATA::SelectFromFolder() const noexcept
{
	// NOT IMPLEMENTED YET
}
TRACK_DATA* PLAYLIST_DATA::RetFirstSelected() const noexcept
{
	for (auto it = tab.begin(); it != tab.end(); it++)
		if ((*it)->selected)
			return *it;

	return 0;
}
void PLAYLIST_DATA::IncPlayed() noexcept
{
	for (auto it = tab.begin(); it != tab.end(); it++)
		if (*it == played)
		{
			if (it == tab.end()-1)
				played = *tab.begin();
			else played = *++it;
			return;
		}
}
void PLAYLIST_DATA::DecPlayed() noexcept
{
	for (auto it = tab.begin(); it != tab.end(); it++)
		if (*it == played)
		{
			if (it == tab.begin())
				played = *(tab.end()-1);
			else played = *--it;
			return;
		}
}
bool PLAYLIST_DATA::IsFirstTrackPlayed() const noexcept
{
	if (!tab.size())
		return false;

	return played == *tab.begin();
}