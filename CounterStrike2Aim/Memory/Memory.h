#pragma once

#include <vector>
#include <iostream>
#include <Windows.h>
#include<TlHelp32.h>


DWORD getProcID(const wchar_t* procName);

uintptr_t getModuleBaseAddress(const wchar_t* procName, DWORD procID);

uintptr_t getDynamicAddress(HANDLE hSnap, uintptr_t addr, std::vector<uintptr_t> offsets);

uintptr_t RPM(HANDLE hProcess, uintptr_t lpBaseAddress);

void WPM(HANDLE hProcess, uintptr_t lpBaseAddress, uintptr_t buffer); //this has to be a template but it is not working



