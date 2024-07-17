#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include "../Offsets/offsets.hpp"
#include "../Offsets/client.dll.hpp"
#include "../Memory/Memory.h"
#include "Game.h"
#include <excpt.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <tchar.h>
#include <eh.h>

template<typename T>
T RPM1(const uintptr_t address, HANDLE handle) noexcept
{
    T rpmBuffer;
    ReadProcessMemory(handle, reinterpret_cast<LPCVOID>(address), &rpmBuffer, sizeof(T), nullptr);
    return rpmBuffer;
}

void SignalHandler(int signal)
{
    printf("Signal %d", signal);
    throw "!Access Violation!";
}

namespace Game {
    DWORD procID = getProcID(L"cs2.exe");
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);
    uintptr_t client = getModuleBaseAddress(L"client.dll", procID);
    uintptr_t localPlayer = RPM(handle, client + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);
    uintptr_t entityList = RPM(handle, client + cs2_dumper::offsets::client_dll::dwEntityList);
    
    namespace LocalPlayer {
        uintptr_t localPlayerIndex = getLocalPlayerIndex();
    }
};

std::vector<uintptr_t> Game::getEntities() {
    std::vector<uintptr_t> entities;
    for (int i = 0; i < 64; i++) {
        try {
            uintptr_t listEntry = RPM1<uintptr_t>(Game::entityList + (8 * (i & 0x7FFF) >> 9) + 0x10, handle);
            if (!listEntry) continue;
            uintptr_t player = RPM1<uintptr_t>(listEntry + 0x78 * (i & 0x1FF), handle);
            if (!player) continue;
            uint32_t playerPawn = RPM1<uint32_t>(player + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn, handle);
            uintptr_t listEntry2 = RPM1<uintptr_t>(Game::entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 0x10, handle);
            uintptr_t pCSPPlayerPawn = RPM1<uintptr_t>(listEntry2 + 0x78 * (playerPawn & 0x1FF), handle);
            int health = RPM1<int>(pCSPPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth, handle);
            if (pCSPPlayerPawn == 0 || pCSPPlayerPawn == Game::localPlayer || health <= 0 || health > 100) continue;
            entities.push_back(pCSPPlayerPawn);
        }catch(const std::exception e){
            continue;
		}
    }
    return entities;
}

Vector3 Game::LocalPlayer::getEyePosition()
{
    Vector3 origin = RPM1<Vector3>(Game::localPlayer + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin, Game::handle);
    std::cout << "Origin: " << origin.x << " " << origin.y << " " << origin.z << std::endl;
    Vector3 eye = RPM1<Vector3>(Game::localPlayer + cs2_dumper::schemas::client_dll::C_BaseModelEntity::m_vecViewOffset, Game::handle);
    return origin + eye;
}

Vector3 Game::Entity::getPosition(uintptr_t entity)
{
    return RPM1<Vector3>(entity + cs2_dumper::schemas::client_dll::C_BasePlayerPawn::m_vOldOrigin, Game::handle);
}

Vector3 Game::Entity::getPosition(uintptr_t entity, int boneID) {
    uintptr_t gameSceneNode = RPM1<uintptr_t>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_pGameSceneNode, Game::handle);
    uintptr_t boneArray = RPM1<uintptr_t>(gameSceneNode + cs2_dumper::schemas::client_dll::CSkeletonInstance::m_modelState + 0x80, Game::handle);
    Vector3 PlayerHead = RPM1<Vector3>(boneArray + (boneID * 32), Game::handle);
    return PlayerHead;
}

Vector3 Game::LocalPlayer::getViewAngles()
{
    return RPM1<Vector3>(Game::client + cs2_dumper::offsets::client_dll::dwViewAngles, Game::handle);
}

uintptr_t Game::LocalPlayer::getLocalPlayerIndex()
{
    for (int i = 0; i < 64; i++) {
            uintptr_t listEntry = RPM1<uintptr_t>(Game::entityList + (8 * (i & 0x7FFF) >> 9) + 0x10, handle);
            if (!listEntry) continue;
            uintptr_t player = RPM1<uintptr_t>(listEntry + 0x78 * (i & 0x1FF), handle);
            if (!player) continue;
            uint32_t playerPawn = RPM1<uint32_t>(player + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn, handle);
            uintptr_t listEntry2 = RPM1<uintptr_t>(Game::entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 0x10, handle);
            uintptr_t pCSPPlayerPawn = RPM1<uintptr_t>(listEntry2 + 0x78 * (playerPawn & 0x1FF), handle);
            if (pCSPPlayerPawn == Game::localPlayer) {
                return i;
            }
    }
}

int Game::LocalPlayer::getLocalPlayerTeam()
{
    return RPM1<int>(Game::localPlayer + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum, Game::handle);
}

int Game::Entity::getHealth(uintptr_t entity)
{
    return RPM1<int>(entity + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth, Game::handle);
}

bool Game::Entity::isVisible(uintptr_t pCSPPlayerPawn)
{
    uintptr_t state = RPM1<uintptr_t>(pCSPPlayerPawn + cs2_dumper::schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + cs2_dumper::schemas::client_dll::EntitySpottedState_t::m_bSpottedByMask, Game::handle);
    return state & (1 << Game::LocalPlayer::localPlayerIndex - 1);
}

int Game::Entity::getEntityTeam(uintptr_t pCSPPlayerPawn)
{
    return RPM1<int>(pCSPPlayerPawn + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum, Game::handle);
}

bool Game::Entity::entityIsTeammate(uintptr_t pCSPPlayerPawn)
{
    return Game::LocalPlayer::getLocalPlayerTeam() == Game::Entity::getEntityTeam(pCSPPlayerPawn);
}


