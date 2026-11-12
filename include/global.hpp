#pragma once
#include<raylib.h>
#include<string>

namespace Config{
    constexpr int FPS=60;
    constexpr int TILE_SIZE=64;
    constexpr int ROW=11;
    constexpr int SCREEN_WIDTH=1200;
    constexpr int SCREEN_HEIGHT=TILE_SIZE * ROW;
}

namespace PlayerConfig{
    constexpr float WIDTH=32.f;
    constexpr float HEIGHT=64.f;
    constexpr float GRAVITY      = 1200.0f; 
    constexpr float MAX_GRAVITY      = 1200.0f; 
    constexpr float JUMP_SPEED   = -600.0f;
    constexpr float PLAYER_SPEED = 400.0f;  
}

namespace OverWorldData
{
    constexpr float PLAYER_SPEED=600.f;
    struct STAGEINFO{
        Vector2 position;
        std::string name;
        int levelIndex;
        int unlocksLevel;
    };
}


