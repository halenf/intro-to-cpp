#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>

Game::Game()
{ }

Game::~Game()
{ }

void Game::Init()
{
    // Used for determining position and movement of game objects
    int posX, posY;
    int velX, velY;

    // Game state
    gameOver = false;
    win = false;
    pause = false;

    shipHeight = (PLAYER_SIZE / 2) / tanf(20 * DEG2RAD);

    // Initialise player
    player.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - shipHeight / 2 };
    player.speed = { 0, 0 };
    player.acceleration = 0;
    player.rotation = 0;
    player.collider = { player.position.x + (float)sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - (float)cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };
    player.colour = RED;

    // Initialise shots
    for (int i = 0; i < PLAYER_MAX_SHOTS; i++)
    {
        // Initialise each shot and make them inactive
        shot[i].position = { 0,0 };
        shot[i].speed = { 0,0 };
        shot[i].radius = SHOT_SIZE;
        shot[i].active = false;
        shot[i].lifeSpawn = 0;
        shot[i].colour = YELLOW;
    }

    // Initialise asteroid
    for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
    {
        int randX[2] = { GetRandomValue(0, player.position.x - 100), GetRandomValue(player.position.x + 100, SCREEN_WIDTH) };
        int randY[2] = { GetRandomValue(0, player.position.y - 100), GetRandomValue(player.position.y + 100, SCREEN_HEIGHT) };
        
        // Set the x position of the asteroid based on the player position, so that the asteroids can't spawn on the player
        if (player.position.x >= 100 && player.position.x <= SCREEN_WIDTH - 100)
        {
            posX = randX[GetRandomValue(0, 1)];
        }
       
    }
}

void Game::Update()
{
    
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    EndDrawing();
}

bool Game::Within100(int pos)
{
    if ()
}