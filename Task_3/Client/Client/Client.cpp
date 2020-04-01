#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <string>

int main()
{
	using namespace std;
	
	HANDLE sem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, LPCWSTR("sem_name"));

	HANDLE file_projection = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, LPCWSTR("file_projection"));
	LPVOID file_pointer = MapViewOfFile(file_projection, FILE_MAP_ALL_ACCESS, 0, 0, 0); //указатель на область памяти виртуального файла

	//HANDLE sem_end = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, LPCWSTR("sem_end"));
	

	while (true) {
		
		WaitForSingleObject(sem, INFINITE);
		
		//if (WaitForSingleObject(sem_end, INFINITE) == WAIT_OBJECT_0) break;

		
		char*str = (char*)malloc(sizeof(char) * 100);
		str = (char*)file_pointer;
		
		if (str == nullptr) cout << "nullptr";
		
		if (!strcmp(str, "q")) break;

		cout << "Client got string:" << endl;
		puts(str);
				
		ReleaseSemaphore(sem, 1, NULL);
	}

	
	UnmapViewOfFile(file_pointer);
	CloseHandle(sem);
	//CloseHandle(sem_end);
	CloseHandle(file_projection);
	return 0;
}



