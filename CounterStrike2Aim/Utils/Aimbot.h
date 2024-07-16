#pragma once
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>
#include "../Offsets/offsets.hpp"
#include "../Offsets/client.dll.hpp"
#include "../Memory/Memory.h"
#include "Vector3.h"
#include "Game.h"
#define AIMSMOOTH  0.2f // can not set greater than 1.0f ! the value must be between 0.0f and 1.0f
#define FOV 5

void aim(Vector3);

Vector3 getClosestEntity();