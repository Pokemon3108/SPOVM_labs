#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <conio.h>
#include "library.h"

using namespace std;
//typedef void(*DLL_LIB_FUNC_READ)(char* buffer, vector<wstring> files);
//typedef void(*DLL_LIB_FUNC_WRITE)(char* text);

void getFiles(vector<wstring>& files);

DWORD WINAPI ThreadReader(LPVOID text);
DWORD WINAPI ThreadWriter(LPVOID text);

//HINSTANCE dll_library;
bool endReadWrite = 0;

int main()
{
	
	char* buffer=(char*)calloc(100, sizeof(char));

	//dll_library = LoadLibraryA("library.dll");
	HANDLE hEvent = CreateEventW(NULL, false, false, L"event");
	
	HANDLE hReader = CreateThread(NULL, 0, ThreadReader, buffer, 0, NULL);
	HANDLE hWriter = CreateThread(NULL, 0, ThreadWriter, buffer, 0, NULL);
	
	
	WaitForSingleObject(hReader, INFINITE);
	CloseHandle(hReader);

	WaitForSingleObject(hWriter, INFINITE);
	CloseHandle(hWriter);

	CloseHandle(hEvent);

	//FreeLibrary(dll_library);
	free(buffer);
	return 0;
}

void getFiles(vector<wstring>& files)
{
	WIN32_FIND_DATA f;
	HANDLE h = FindFirstFileW(L"..\\Client\\TextFiles\\*", &f);
	if (h != INVALID_HANDLE_VALUE) {
		do {
			wstring fName = wstring(f.cFileName);
			if (fName == L".." || fName == L"." || fName == L"Result.txt")
				continue;

			files.push_back(f.cFileName);
		} while (FindNextFileW(h, &f));
	}
	else {
		cerr << "Error opening directory" << endl;
	}

}


DWORD WINAPI ThreadWriter(LPVOID text)
{

	write((char*)text);
	return 0;
	
}

DWORD WINAPI ThreadReader(LPVOID text) {
	
	read((char*)text);
	
	return 0;
}

