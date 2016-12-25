#include "playlist.h"

void PLAYLIST::StartDragNDrop(HDROP hDrop) noexcept
{
	try
	{
		DND_DATA* data = new DND_DATA(TRACK_DATA::hwnd, hDrop);

		DWORD ThreadId;
		HANDLE hThread = CreateThread(0, 0, DragNDropThreadFunction, reinterpret_cast<void*>(data), 0, &ThreadId);
		SendMessage(TRACK_DATA::hwnd, WM_LOADING_FACTOR, 1, 0);
		CloseHandle(hThread);
	}
	catch(...) { }
}
void LoadRecursive(std::wstring& path, std::vector<TRACK_DATA*>* inserted) noexcept
{
	WIN32_FIND_DATAW data;
	HANDLE hFound = FindFirstFileW(path.c_str(), &data);

	// podana œcie¿ka to folder
	if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		std::wstring folder_path(path, 0, path.size()-1);
		folder_path += L"\\*";

		hFound = FindFirstFileW(folder_path.c_str(), &data);
		while(FindNextFileW(hFound, &data))
		{
			if (!lstrcmpW(L".", data.cFileName) || !lstrcmpW(L"..", data.cFileName))
				continue;

			std::wstring new_file_path(folder_path, 0, folder_path.size()-1);
			new_file_path += std::wstring(data.cFileName) + L'\0';
			LoadRecursive(new_file_path, inserted);
		}
	}
	// podana œcie¿ka to plik
	else
	{
		try
		{
			TRACK_DATA* track = new TRACK_DATA(path);
			inserted->push_back(track);
		}
		catch(...) {}
	}

	FindClose(hFound);
}
DWORD WINAPI DragNDropThreadFunction(void* arg) noexcept
{
	DND_DATA* data = reinterpret_cast<DND_DATA*>(arg);
	unsigned hdrop_count = DragQueryFileW(data->hDrop, 0xffffffff, 0, 0);
	data->inserted = new std::vector<TRACK_DATA*>();

	for (unsigned i = 0; i < hdrop_count; i++)
	{
		unsigned size = DragQueryFileW(data->hDrop, i, 0, 0)+1;
		std::wstring path(size, 0);
		DragQueryFileW(data->hDrop, i, const_cast<wchar_t*>(path.c_str()), size);
		path.erase(path.end());
		LoadRecursive(path, data->inserted);
	}

	PostMessage(data->hwnd, PLAYLIST::WM_DROPFILESLOADED, reinterpret_cast<WPARAM>(arg), 0);
	return 0;
}
void PLAYLIST::FinishDragNDrop(WPARAM wParam, LPARAM lParam) noexcept
{
	DND_DATA* dnd = reinterpret_cast<DND_DATA*>(wParam);
	data.tab.insert(*reinterpret_cast<std::vector<TRACK_DATA*>::iterator*>(lParam), dnd->inserted->begin(), dnd->inserted->end());
	delete dnd->inserted;
	DragFinish(dnd->hDrop);
	delete dnd;

	SendMessage(TRACK_DATA::hwnd, WM_LOADING_FACTOR, 0, 0);
}