#include "Aimbot.h"
#include <math.h>

template<typename T>
T RPM1(const uintptr_t address, HANDLE handle) noexcept
{
    T rpmBuffer;
    ReadProcessMemory(handle, reinterpret_cast<LPCVOID>(address), &rpmBuffer, sizeof(T), nullptr);
    return rpmBuffer;
}

void aim(Vector3 closestEnemy)
{

    Vector3 viewAngles = Game::LocalPlayer::getViewAngles();
    Vector3 localPlayerPosition = Game::LocalPlayer::getEyePosition();
    //Vector3 closestEnemy = getClosestEntity(game);

    float pitch = localPlayerPosition.pitchAngle(closestEnemy);
    float yaw = localPlayerPosition.yawAngle(closestEnemy);

    Vector3 newAngles = Vector3(pitch, yaw, 0.0f);
    Vector3 delta = viewAngles.delta(newAngles);

    //ADDING SMOOTHNESS
    Vector3 smoothAngles = viewAngles + delta * AIMSMOOTH;
    std::cout << "Smooth Angles: " << smoothAngles.x << " " << smoothAngles.y << std::endl;

    WriteProcessMemory(Game::handle, reinterpret_cast<void*>(Game::client + cs2_dumper::offsets::client_dll::dwViewAngles), &smoothAngles, sizeof(smoothAngles), NULL);
}

Vector3 getClosestEntity()
{
    std::vector<uintptr_t> playerList = Game::getEntities();
    Vector3 closestEnemy = Vector3(0, 0, 0);
    boolean first = true;
    Vector3 localPlayerPosition = Game::LocalPlayer::getEyePosition();
    Vector3 localPlayerViewAngles = Game::LocalPlayer::getViewAngles();

    for (auto player : playerList)
    {

        Vector3 playerPosition = Game::Entity::getPosition(player,6); // 6 is for head position
        int health = Game::Entity::getHealth(player);

        if (first || ((health > 0 || health <= 100) && playerPosition.magnitude(localPlayerPosition) < closestEnemy.magnitude(localPlayerPosition)))
        {
            //FOV CHECK !
            float pitch = localPlayerPosition.pitchAngle(playerPosition);
            float yaw = localPlayerPosition.yawAngle(playerPosition);
            Vector3 newAngles = Vector3(pitch, yaw, 0.0f);
            Vector3 delta = localPlayerViewAngles.delta(newAngles);
            float norm = sqrt(delta.x * delta.x + delta.y * delta.y);
            if (delta.normalize() > FOV) continue;

            closestEnemy = playerPosition;
            first = false;
        }
    }
    return closestEnemy;
}