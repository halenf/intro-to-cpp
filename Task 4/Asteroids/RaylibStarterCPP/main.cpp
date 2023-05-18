#include "Game.h"
#include "raylib.h"

int main(int argc, char* argv[])
{
    // Screen dimensions
    int screenWidth = SCREEN_WIDTH;
    int screenHeight = SCREEN_HEIGHT;
    
    // Create window
    InitWindow(screenWidth, screenHeight, "Blasteroids");

    // Set framerate target
    SetTargetFPS(FPS);
    
    // Instantiate game class
    Game game;

    // Game loop
    while (!WindowShouldClose())
    {
        game.UpdateDrawFrame();
    }

    // Unload textures, sounds, etc. and exit game
    game.Unload();
    CloseWindow();

    return 0;
}