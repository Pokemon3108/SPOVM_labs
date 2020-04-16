#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <string>

#define client_name "D:\\Учеба\\Лабораторные СПОВМ\\Task_3\\Client\\Debug\\Client.exe"
using namespace std;

HANDLE createProcess();


int main()
{
	HANDLE sem = CreateSemaphore(NULL, 0, 1, LPCWSTR("sem_name"));
	if (!sem) {
		cout << "Semaphore was not created" << endl;
	}

	
	HANDLE file_projection = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 100, LPCWSTR("file_projection"));
	if (!file_projection) {
		cout << "File projection was not created" << endl;
		return 0;
	}

	LPVOID file_pointer=MapViewOfFile(file_projection, FILE_MAP_ALL_ACCESS, 0, 0, 0); //указатель на область памяти виртуального файла
	if (!file_pointer) {
		cout << "Pointer for memory was not gotten" << endl;
		return 0;
	}

	
	/*HANDLE sem_end = CreateSemaphore(NULL, 0, 1, LPCWSTR("sem_end"));
	if (!sem_end) {
		cout << "Semaphore for stopping program was not created" << endl;
	}*/

	//HANDLE sems[2] = { sem_end,sem };

	HANDLE child=createProcess();
	

	while (true) {
		cout << "Input string (server) 'q' for stop" << endl;
		string str;
		getline(cin, str);
		
		strcpy((char*)file_pointer, const_cast<char*>(str.c_str()));
		
		if (str == "q") break;
		
		ReleaseSemaphore(sem, 1, NULL);
		
		WaitForSingleObject(sem, INFINITE);
	}

	//ReleaseSemaphore(sem_end, 1, NULL);
	ReleaseSemaphore(sem, 1, NULL);
	WaitForSingleObject(child,INFINITE); //ждем клиентский процесс
	CloseHandle(child);
	UnmapViewOfFile(file_pointer);
	CloseHandle(sem);
	//CloseHandle(sem_end);
	CloseHandle(file_projection);
	return 0;

}



HANDLE createProcess() {
	STARTUPINFO info;
	ZeroMemory(&info, sizeof(info));
	info.cb = sizeof(info);

	PROCESS_INFORMATION proc_info;
	ZeroMemory(&proc_info, sizeof(proc_info));

	if (!CreateProcess(TEXT(client_name),
		NULL, //???
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&info,
		&proc_info)) 
	{
		cout << "Process wasn't created" << endl;
	}

	return proc_info.hProcess; //дескриптор процесса
}



