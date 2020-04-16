#include "pch.h"
#include <iostream>
#include <list>
#include <Windows.h>
#include <string>
#include <vector>

HANDLE createThread(DWORD* number);
DWORD WINAPI printProcessInformation(LPVOID number);

CRITICAL_SECTION criticalSection;

DWORD closeID;

std::vector<HANDLE> events;
int counter = 0;

int main()
{
	using namespace std;
	list<HANDLE> l;
	HANDLE hThread;
	DWORD number=0;

	InitializeCriticalSection(&criticalSection);

	while (true) {
		char c = getchar();
		switch (c) {
		case '+':
		{
			hThread = createThread(&number);
			++number;
			l.push_back(hThread);

			string eventName = to_string(number);
			HANDLE hEvent = CreateEvent(NULL, false, false, LPTSTR(eventName.c_str()));
			events.push_back(hEvent);

			if (events.size() == 1) SetEvent(hEvent);
			break;
		}

		case '-':
			if (l.empty()) cout << "No threads left" << endl;
			else {
				//--number;
				closeID = GetThreadId(l.back());
				CloseHandle(l.back());
				l.pop_back();
			}
			break;

		case 'q':
			while (!l.empty()) {
				closeID = GetThreadId(l.back());
				CloseHandle(l.back());
				l.pop_back();
			}
			DeleteCriticalSection(&criticalSection);
			return 0;
		}

		Sleep(500);
		rewind(stdin);
	}

	
	
}

DWORD WINAPI printProcessInformation(LPVOID number)
{
	using namespace std;
	int process_number = *(DWORD*)number;
	string info = "Thread:"+to_string(process_number);
	string eventName = to_string(process_number);
	HANDLE eventCurrentThread = OpenEvent(EVENT_ALL_ACCESS, false, LPTSTR(eventName.c_str()));
	while (true) {
		if (closeID == GetCurrentThreadId()) return 0;


		WaitForSingleObject(eventCurrentThread,INFINITE);
		EnterCriticalSection(&criticalSection);
		for (int i = 0; i < info.length(); ++i)
			cout << info[i];
		cout << "\n";
		if (counter != events.size()-1) ++counter;
		else counter = 0;
		LeaveCriticalSection(&criticalSection);	

		
		SetEvent(events[counter]);
		Sleep(1000);
	}
	
	return 0;

}

HANDLE createThread(DWORD* number) {
	DWORD id;
	HANDLE hThread;
	hThread = CreateThread(NULL, 0, printProcessInformation, number, 0, &id);
	if (!hThread) {
		std::cout << "Thread was not created";
		return 0;
	}
	
	return hThread;
}
