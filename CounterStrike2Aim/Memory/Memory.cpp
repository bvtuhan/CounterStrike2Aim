#include "Memory.h"
#include <vector>
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <Windows.h>
#include <Psapi.h>
#include <string>


DWORD getProcID(const wchar_t* procName) {
	DWORD procID = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE) {
		PROCESSENTRY32 p32;
		p32.dwSize = sizeof(p32);
		if (Process32First(hSnap, &p32)) {
			do {
				if (!_wcsicmp(procName, p32.szExeFile)) {
					procID = p32.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &p32));
		}
	}
	CloseHandle(hSnap);
	return procID;
}

uintptr_t getModuleBaseAddress(const wchar_t* procName, DWORD procID) {
	uintptr_t ModuleBaseAddress = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 p32;
		p32.dwSize = sizeof(p32);
		if (Module32First(hSnap, &p32)) {
			do {
				if (!_wcsicmp(procName, p32.szModule)) {
					ModuleBaseAddress = (uintptr_t)p32.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &p32));
		}
	}
	CloseHandle(hSnap);
	return ModuleBaseAddress;
}

uintptr_t getDynamicAddress(HANDLE hSnap, uintptr_t addr, std::vector<uintptr_t> offsets)
{
	DWORD ptr = addr;
	for (unsigned int i = 0; i < offsets.size(); i++) {
		ReadProcessMemory(hSnap, (LPCVOID)addr, &addr, sizeof(addr), 0);
		addr += offsets.at(i);
	}
	return addr;
}

void WPM(HANDLE hProcess, uintptr_t lpBaseAddress, uintptr_t buffer)
{
	WriteProcessMemory(hProcess, (LPVOID)lpBaseAddress, &buffer, sizeof(buffer), 0);
}

uintptr_t RPM(HANDLE hProcess, uintptr_t lpBaseAddress) {
	uintptr_t buffer;
	ReadProcessMemory(hProcess, (LPCVOID)lpBaseAddress, &buffer, sizeof(buffer), 0);
	return buffer;
}



