#include "stdafx.h"
#include "library.h"

using namespace std;

bool endReadWrite = 0;


void read(char* buffer) {
	
	HANDLE event = OpenEventW(EVENT_ALL_ACCESS, false, L"event");
	HANDLE hFile;
	DWORD fileSize;

	vector<wstring> files;
	getFiles(files);

	OVERLAPPED asynch;
	asynch.Offset = 0;
	asynch.OffsetHigh = 0;
	asynch.hEvent = event;

	vector<wstring>::iterator it;
	for (it = files.begin(); it != files.end(); ++it) {
		wstring fileName = L"D:\\Учеба\\Лабораторные СПОВМ\\Task_5\\Client\\Client\\TextFiles\\";
		fileName += *it;
		hFile = CreateFileW(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, &asynch);
		fileSize = GetFileSize(hFile, NULL);

		ReadFile(hFile, buffer, fileSize, NULL, &asynch);
		CloseHandle(hFile);
		Sleep(200);
		WaitForSingleObject(event, INFINITE);
		for (int i = 0; i < strlen(buffer) && buffer[i]; ++i)
			buffer[i] = '\0';


	}

	SetEvent(event);
	endReadWrite = 1;
}

void write(char* text) {

	
	HANDLE event = OpenEventW(EVENT_ALL_ACCESS, false, L"event");

	OVERLAPPED asynch;
	asynch.Offset = 0;
	asynch.OffsetHigh = 0;
	asynch.hEvent = event;

	HANDLE resultFile = CreateFileW(L"D:\\Учеба\\Лабораторные СПОВМ\\Task_5\\Client\\Client\\TextFiles\\Result.txt",
		GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, &asynch);

	while (!endReadWrite) {

		WaitForSingleObject(event, INFINITE);

		int length = strlen(text);

		WriteFile(resultFile, text, length, NULL, &asynch);

		asynch.Offset += length;
		Sleep(300);
	}


	CloseHandle(resultFile);
}

void getFiles(vector<wstring>& files)
{
	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFileW(L"D:\\Учеба\\Лабораторные СПОВМ\\Task_5\\Client\\Client\\TextFiles\\*", &f);
	if (h != INVALID_HANDLE_VALUE) {
		do {
			wstring fName = wstring(f.cFileName);
			if (fName == L".." || fName == L"." || fName == L"Result.txt")
				continue;

			files.push_back(f.cFileName);
		} while (FindNextFileW(h, &f));
	}
	
}





