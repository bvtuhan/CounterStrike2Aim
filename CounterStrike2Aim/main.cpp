#include <iostream>
#include "Memory.h"
#include <vector>
#include <string>
#include "Memory/Memory.h"
#include "Offsets/client.dll.hpp"
#include "Offsets/offsets.hpp"
#include "Utils/Game.h"
#include "Utils/Vector3.h"
#include <Windows.h>
#include <TlHelp32.h>
#include "Utils/Aimbot.h"

template <class T>
T RPM(const uintptr_t address, HANDLE handle) noexcept
{
    T rpmBuffer;
    ReadProcessMemory(handle, reinterpret_cast<LPCVOID>(address), &rpmBuffer, sizeof(T), nullptr);
    return rpmBuffer;
}

int main()
{
    std::vector<uintptr_t> playerList = Game::getEntities();

    uintptr_t localPlayer = Game::localPlayer;
    Vector3 localPlayerPosition = Game::LocalPlayer::getEyePosition();
    Vector3 localPlayerViewAngles = Game::LocalPlayer::getViewAngles();
    std::cout << "Local Player position : x: " << localPlayerPosition.x << " y: " << localPlayerPosition.y << " z: " << localPlayerPosition.z << std::endl;
    std::cout << "Local Player View Angles : x: " << localPlayerViewAngles.x << " y: " << localPlayerViewAngles.y << std::endl;

    for (uintptr_t player : playerList) {
        Vector3 playerPosition = Game::Entity::getPosition(player,6);
        std::cout << "Player Health: " << RPM<int>(player + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth, Game::handle) << std::endl;
        std::cout << "Player Position: x: " << playerPosition.x << " y: " << playerPosition.y << " z: " << playerPosition.z << std::endl;
    }

    std::cout << "LocalPlayerIndex : " << Game::LocalPlayer::localPlayerIndex << std::endl;

    while (31)
    {
        if (GetAsyncKeyState(VK_XBUTTON1))
        {
            break;
        }

        while (GetAsyncKeyState(VK_MENU))
        {
            Vector3 closestEnemy = getClosestEntity();
            std::cout << "Closest Enemey is in" << " x: " << closestEnemy.x << " y: " << closestEnemy.y << std::endl;
            if (closestEnemy.x == 0 && closestEnemy.y == 0 && closestEnemy.z == 0) continue;
            aim(closestEnemy);
        }
    }



    return 0;
}
