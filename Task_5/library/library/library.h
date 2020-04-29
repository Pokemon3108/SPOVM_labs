#pragma once
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

extern "C" __declspec(dllexport) void read(char* buffer);
extern "C" __declspec(dllexport) void write(char* text);
extern "C" __declspec(dllexport) void getFiles(vector<wstring>& files);

