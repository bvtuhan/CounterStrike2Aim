#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include "../Offsets/offsets.hpp"
#include "../Offsets/client.dll.hpp"
#include "../Memory/Memory.h"
#include "Vector3.h"

template<typename T>
extern T RPM(HANDLE handle, uintptr_t address);

namespace Game {
	std::vector<uintptr_t> getEntities();

	extern DWORD procID;
	extern HANDLE handle;
	extern uintptr_t client;
	extern uintptr_t localPlayer;
	extern uintptr_t entityList;

	namespace LocalPlayer {
		extern uintptr_t localPlayerIndex;
		Vector3 getEyePosition();
		Vector3 getViewAngles();
		uintptr_t getLocalPlayerIndex();
	};

	namespace Entity {
		Vector3 getPosition(uintptr_t entity);
		Vector3 getPosition(uintptr_t entity, int bone);
		int getHealth(uintptr_t entity);
		bool isVisible(uintptr_t pCSPPlayerPawn);
	};
};