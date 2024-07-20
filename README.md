# CounterStrike2Aim
since vac does check opening handle to game from other processes and memory places where that memory should be untouched, using this will get you probably insta banned (unless, if you know what you are doing, you can bypass it with just few steps, for exampl. handle hijacking + kernel read/write). so only for educational purposes, not responsible for any bans on your accounts

## Installation

- compile it with visual studio (c++17)
- if needed, update the `client.dll.hpp` and `offsets.hpp` from  [cs2-dumper](https://bulldogjob.com/news/449-how-to-write-a-good-readme-for-your-github-project) (since im too lazy to fetch dynamically)
- even though vac sucks, it can still clap your cheeks, so do not just compile and grind, you would probably marked with red trust and then banned, thus only for educational purposes, not responsible for any bans on your accounts

## Customization
- to adjust the fov and smoothness in `Aimbot.h`
```cpp
 #define AIMSMOOTH  0.2f // must between 0 and 1
 #define FOV 5
```
- you can change the aimbot key in `main.cpp` (check [Winuser.h](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes))

```cpp
while (GetAsyncKeyState(VK_MENU))
{
    //los gehts
}
```
- to turn off visibility and teammate check just command out these lines accordingly in `Aimbot.cpp`
```cpp
Vector3 getClosestEntity()
{
    for (auto player : playerList)
    {
        ...
        if(!Game::Entity::isVisible(player)) continue; <--
        if(Game::Entity::entityIsTeammate(player)) continue; <--
        ...
    }
    return closestEnemy;
}
```


## Credits

- [cs2-dumper](https://github.com/a2x/cs2-dumper)
- [unknowncheats](https://www.unknowncheats.me/forum/counter-strike-2-a/)
