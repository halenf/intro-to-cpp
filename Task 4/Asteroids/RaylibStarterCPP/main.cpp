#include "Game.h"
#include "raylib.h"

int main(int argc, char* argv[])
{
    // Screen dimensions
    int screenWidth = SCREEN_WIDTH;
    int screenHeight = SCREEN_HEIGHT;
    
    // Create window
    InitWindow(screenWidth, screenHeight, "Asteroids");

    // Set framerate target
    SetTargetFPS(60);
    
    // Instantiate game class
    Game game;

    // Run Game::Init()
    game.Init();

    // Game loop
    while (!WindowShouldClose())
    {
        game.Update();
        game.Draw();
    }

    // Unload textures, sounds, etc. and exit game
    game.Unload();
    CloseWindow();

    return 0;
}