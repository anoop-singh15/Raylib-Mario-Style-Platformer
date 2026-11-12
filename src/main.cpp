#include <raylib.h>
#include "include/global.hpp"
#include "include/game.hpp"



int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, "Treaure Hunter");
    InitAudioDevice();
    SetTargetFPS(Config::FPS);

    // Disable the default ESC key from closing the window
    SetExitKey(KEY_NULL);
    
    Game game;

    while (!WindowShouldClose())
    {
        float dT = GetFrameTime();
        if (dT > 0.05f)
        {
            dT = 0.05f;
        }

        game.Update(dT);

        BeginDrawing();
        ClearBackground(GRAY);

        game.Draw();

        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
}
