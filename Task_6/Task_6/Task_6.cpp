#include "pch.h"
#include <iostream>
#include <windowsx.h>
#include <Windows.h>
#include <stdio.h>

using namespace std;
void* myMalloc(int size);
void myFree(void* ptr);


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
	
}

void * myMalloc(int size)
{
	void* ptr;
	HLOCAL memoryHandle;
	memoryHandle = LocalAlloc(LMEM_MOVEABLE, size+sizeof(HLOCAL));
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
	LocalFree(memoryHandle);
	if ( LocalFree(memoryHandle) ) ptr = nullptr;
	
}


