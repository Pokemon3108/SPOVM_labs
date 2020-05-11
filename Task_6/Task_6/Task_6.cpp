#include "pch.h"
#include <iostream>
#include <windowsx.h>
#include <Windows.h>
#include <stdio.h>

using namespace std;
void* myMalloc(int size);
void myFree(void* ptr);
void* mallocSwap(int size);
void freeSwap(void* ptr);
DWORD getGranularity();

DWORD offset = 0;
HANDLE hFile = INVALID_HANDLE_VALUE;

int main() {

	char* str = (char*)myMalloc(20);
	if (!str) {
		cout << "Memory location failed" << GetLastError() << endl;
		return 1;
	}
	else cout << "Input string" << endl;

	gets_s(str, 20);
	puts(str);

	myFree(str);
	if (str != nullptr) cout << "Free failed" << endl;

	int* arr1 = (int*)mallocSwap(20 * sizeof(int));
	cin >> arr1[0];
	cout << arr1[0];


	char*str2 = (char*)mallocSwap(10);
	gets_s(str2, 10);

	freeSwap(str2);
	freeSwap(arr1);

	CloseHandle(hFile);

	return 0;
}

void * myMalloc(int size)
{
	void* ptr;
	HLOCAL memoryHandle;
	memoryHandle = LocalAlloc(LMEM_MOVEABLE, size + sizeof(HLOCAL));
	if (!memoryHandle)  return nullptr;

	ptr = LocalLock(memoryHandle);
	ptr = (char*)ptr + sizeof(HLOCAL);
	return ptr;

}

void myFree(void * ptr)
{
	HLOCAL memoryHandle;
	memoryHandle = (char*)ptr - sizeof(HLOCAL);

	LocalUnlock(memoryHandle);
	if (LocalFree(memoryHandle)) ptr = nullptr;

}

void* mallocSwap(int size)
{
	if (hFile == INVALID_HANDLE_VALUE) hFile = CreateFileMappingW(hFile, NULL, PAGE_EXECUTE_READWRITE, 0, 2147483648, L"swap_file");

	void* ptr;
	ptr = MapViewOfFile(hFile, FILE_MAP_ALL_ACCESS, 0, offset, size);

	DWORD granuality = getGranularity();
	offset += granuality + size / granuality;

	return ptr;
}

void freeSwap(void * ptr)
{
	UnmapViewOfFile(ptr);
}

DWORD getGranularity()
{
	DWORD granuality;
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	granuality = info.dwAllocationGranularity;
	return granuality;
}
