#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <random>
#include <string>

Game::Game()
{ }

Game::~Game()
{ }

void Game::Init()
{
    // Used for determining position and velocity of game objects
    float posX, posY;
    float velX, velY;

    // Game state
    gameOver = false;
    win = false;
    pause = false;

    shipHeight = (PLAYER_SIZE / 2) / tanf(20 * DEG2RAD);

    // Initialise player
    player.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - shipHeight / 2 };
    player.velocity = { 0, 0 };
    player.acceleration = 0;
    player.rotation = 0;
    player.collider = { player.position.x + (float)sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - (float)cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };
    player.colour = RED;

    // Initialise shots
    shots.clear();
    shots.reserve(20);

    // Initialise asteroids
    int arraySet[3] = { 0, 0, 0 };
    std::copy(std::begin(arraySet), std::end(arraySet), asteroidCount); // Reset asteroidCount

    for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
    {
        // Generate random asteroid type
        // Ensure an asteroid of a certain type is not created if there is already a maximum of that type
        int type = GetRandomValue(0, 2);
        while (asteroidCount[type] >= maxAsteroid[type])
        {
            type = GetRandomValue(0, 2);
        }
        asteroidCount[type] += 1;
        
        // Used for ensuring that asteroids do not spawn within 100 units of the player
        // If the possible spawn location for an asteroid is not within the range of the screen, this allows it to 'overflow'
        float playerX = player.position.x;
        if (playerX + 100 > SCREEN_WIDTH)
        {
            playerX -= SCREEN_WIDTH;
        }
        if (playerX - 100 < 0)
        {
            playerX += SCREEN_WIDTH;
        }

        float playerY = player.position.y;
        if (playerY + 100 > SCREEN_HEIGHT)
        {
            playerY -= SCREEN_HEIGHT;
        }
        if (playerY - 100 < 0)
        {
            playerY += SCREEN_HEIGHT;
        }
        
        // Sets ranges for position that are at least 100 units away from the player
        float randPosX[2] = { GetRandomValue(0, playerX - 100), GetRandomValue(playerX + 100, SCREEN_WIDTH) };
        float randPosY[2] = { GetRandomValue(0, playerY - 100), GetRandomValue(playerY + 100, SCREEN_HEIGHT) };

        posX = randPosX[GetRandomValue(0, 1)];
        posY = randPosY[GetRandomValue(0, 1)];

        // Randomly set velocity based on asteroid type and asteroid base speed
        // Both cannot be 0 (that would make the asteroid stationary
        velX = 0;
        velY = 0;

        while (velX == 0 || velY == 0)
        {
            velX = GetRandomValue(-ASTEROID_BASE_SPEED * (type + 1), ASTEROID_BASE_SPEED * (type + 1)) / 100;
            velY = GetRandomValue(-ASTEROID_BASE_SPEED * (type + 1), ASTEROID_BASE_SPEED * (type + 1)) / 100;
        }

        // Set radius based on asteroid type
        int radius = ASTEROID_BASE_RADIUS * (type + 1);

        // Set health based on asteroid type
        float health = ASTEROID_BASE_HEALTH * (type + 1);
        
        // Set colour based on type
        float colourValue = 110 + 40 * type + GetRandomValue(0, 65);
        Color colour = { colourValue, colourValue, colourValue, 255 };

        asteroids[i].position = { posX, posY };
        asteroids[i].velocity = { velX, velY };
        asteroids[i].radius = radius;
        asteroids[i].type = (AsteroidType)type;
        asteroids[i].health = health;
        asteroids[i].colour = colour;
        asteroids[i].active = true;
    }
}

void Game::Update()
{
    if (!gameOver)
    {
        // Toggles pause if 'P' is pressed
        if (IsKeyPressed(KEY_P)) pause = !pause;

        if (!pause)
        {
            // Player logic
            if (IsKeyDown(KEY_LEFT)) player.rotation -= PLAYER_ROTATION_SPEED;
            if (IsKeyDown(KEY_RIGHT)) player.rotation += PLAYER_ROTATION_SPEED;

            player.velocity = { sin(player.rotation * DEG2RAD) * PLAYER_SPEED, -cos(player.rotation * DEG2RAD) * PLAYER_SPEED };

            if (IsKeyDown(KEY_UP))
            {
                if (player.acceleration < 1) player.acceleration += playerAcceleration;
            }
            else
            {
                if (player.acceleration > 0) player.acceleration -= DRAG_CONSTANT;
                else if (player.acceleration < 0) player.acceleration = 0;
            }

            if (IsKeyDown(KEY_DOWN))
            {
                if (player.acceleration > 0) player.acceleration -= playerAcceleration;
                else if (player.acceleration < 0) player.acceleration = 0;
            }

            player.position.x += player.velocity.x * player.acceleration;
            player.position.y += player.velocity.y * player.acceleration;

            if (player.position.x > SCREEN_WIDTH + shipHeight) player.position.x = -shipHeight;
            else if (player.position.x < -shipHeight) player.position.x = SCREEN_WIDTH + shipHeight;
            if (player.position.y > SCREEN_HEIGHT + shipHeight) player.position.y = -shipHeight;
            else if (player.position.y < -shipHeight) player.position.y = SCREEN_HEIGHT + shipHeight;

            // Shooting logic
            if (IsKeyPressed(KEY_SPACE))
            {
                if (shots.size() == shots.capacity())
                {
                    EraseShot(shots.begin());
                }
                Shot shot;
                shot.position = { player.position.x + sin(player.rotation * DEG2RAD) * shipHeight, player.position.y - cos(player.rotation * DEG2RAD) * shipHeight };
                shot.velocity = { 1.5f* sin(player.rotation * DEG2RAD) * PLAYER_SPEED, 1.5f * -cos(player.rotation * DEG2RAD) * PLAYER_SPEED }; 
                shot.rotation = player.rotation;
                shot.radius = SHOT_SIZE;
                shot.colour = YELLOW;
                shot.lifeTime = 0;
                shots.push_back(shot);
            }

            for (int i = 0; i < shots.size(); i++)
            {
                shots[i].position.x += shots[i].velocity.x;
                shots[i].position.y += shots[i].velocity.y;

                if (shots[i].position.x < -shots[i].radius / 2)
                {
                    EraseShot(shots[i]);
                    break;
                }
                if (shots[i].position.x > SCREEN_WIDTH + shots[i].radius / 2)
                {
                    EraseShot(shots[i]);
                    break;
                }
                if (shots[i].position.y < -shots[i].radius / 2)
                {
                    EraseShot(shots[i]);
                    break;
                }
                if (shots[i].position.y > SCREEN_HEIGHT + shots[i].radius / 2)
                {
                    EraseShot(shots[i]);
                    break;
                }
            }

            for (int i = 0; i < shots.size(); i++)
            {
                if (shots[i].lifeTime >= SHOT_LIFE_TIME)
                {
                    EraseShot(shots[i]);
                    break;
                }
                shots[i].lifeTime++;
            }

            // Meteor logic
            for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
            {
                asteroids[i].position.x += asteroids[i].velocity.x;
                asteroids[i].position.y += asteroids[i].velocity.y;

                if (asteroids[i].position.x > SCREEN_WIDTH + asteroids[i].radius) asteroids[i].position.x = -asteroids[i].radius;
                else if (asteroids[i].position.x < -asteroids[i].radius) asteroids[i].position.x = SCREEN_WIDTH + asteroids[i].radius;
                if (asteroids[i].position.y > SCREEN_HEIGHT + asteroids[i].radius) asteroids[i].position.y = -asteroids[i].radius;
                else if (asteroids[i].position.y < -asteroids[i].radius) asteroids[i].position.y = SCREEN_HEIGHT + asteroids[i].radius;

                if (asteroids[i].health <= 0)
                {
                    asteroids[i].active = false;
                }
            }

            // Collisions
            player.collider = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };

            for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
            {
                if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, asteroids[i].position, asteroids[i].radius) && asteroids[i].active) gameOver = true;
            }

            for (int i = 0; i < shots.size(); i++)
            {
                for (int j = 0; j < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; j++)
                {
                    if (asteroids[j].active && CheckCollisionCircles(shots[i].position, shots[i].radius, asteroids[j].position, asteroids[j].radius))
                    {
                        std::cout << "Shot at position " << shots[i].position.x << ", " << shots[i].position.y << " collided with asteroid at position " << asteroids[j].position.x << ", " << asteroids[j].position.y << std::endl; // debug
                        asteroids[j].health -= SHOT_DAMAGE;
                        if (asteroids[j].health <= 0)
                        {
                            asteroids[j].active = false;
                        }
                        else
                        {
                            asteroids[j].SetVelocity(Vector2Subtract(shots[i].velocity, asteroids[j].velocity));
                            
                            /*
                            Vector2 direction = Vector2Subtract(shots[i].velocity, asteroids[j].velocity); // Vector representing the resultant vector of the bullet-asteroid collision
                            float a = 1 / (((float)asteroids[j].type + 1) * INELASTIC_CONSTANT); // Asteroid type / amplitude
                            float x = Vector2Length(direction); // Length of the resultant vector 'direction' / variable
                            float b = Vector2Length(shots[i].velocity) + Vector2Length(asteroids[j].velocity); // Represents the maximum amount of momentum that can be transferred
                                                                                                               // to the asteroid / period
                            std::cout << "Resultant vector length: " << x << std::endl;
                            std::cout << "Maximum vector length: " << b << std::endl;
                            float multiplier = a * cos(((2 * PI) / b) * x) + a; // Provides a 0-1 multiplier (affected by asteroid type) that changes how much of the bullet's momentum
                                                                                // is transferred in the collision. Will be higher if the collision is direct rather than just a "glance".
                            std::cout << "Momentum transfer multiplier: " << multiplier << std::endl; // debug
                            asteroids[j].AddVelocity(shots[i].velocity.x * multiplier, shots[i].velocity.y * multiplier); // Transfer the momentum to the asteroid
                            */
                        }
                        EraseShot(shots[i]);
                        break;
                    }
                }
            }

            // Game state logic
            for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
            {
                if (asteroids[i].active == true)
                {
                    break;
                }
                win = true;
            }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            std::cout << "Game restarting" << std::endl;
            Init();
        }
        if (IsKeyPressed(KEY_ESCAPE))
        {
            CloseWindow();
        }
    }
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    if (!gameOver)
    {
        // Draw player
        Vector2 v1 = { player.position.x + sinf(player.rotation * DEG2RAD) * (shipHeight), player.position.y - cosf(player.rotation * DEG2RAD) * (shipHeight) };
        Vector2 v2 = { player.position.x - cosf(player.rotation * DEG2RAD) * (PLAYER_SIZE / 2), player.position.y - sinf(player.rotation * DEG2RAD) * (PLAYER_SIZE / 2) };
        Vector2 v3 = { player.position.x + cosf(player.rotation * DEG2RAD) * (PLAYER_SIZE / 2), player.position.y + sinf(player.rotation * DEG2RAD) * (PLAYER_SIZE / 2) };
        DrawTriangle(v1, v2, v3, RED);
        DrawCircle(player.position.x, player.position.y, 4, RED);

        //DrawTextureEx(); // For using sprite instead of circle

        // Draw shots
        for (int i = 0; i < shots.size(); i++)
        {
            DrawCircle(shots[i].position.x, shots[i].position.y, shots[i].radius, shots[i].colour);
        }

        // Draw asteroids
        for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
        {
            if (asteroids[i].active) DrawCircle(asteroids[i].position.x, asteroids[i].position.y, asteroids[i].radius, asteroids[i].colour);
        }

        // Draw FPS
        DrawText(std::to_string(GetFPS()).c_str(), 0, 0, 24, WHITE);

        // Pause text
        if (pause)
        {
            DrawText("Paused", SCREEN_WIDTH / 2 - MeasureText("Paused", 48) / 2, SCREEN_HEIGHT / 2, 48, WHITE);
        }

        // Win text
        if (win)
        {
            DrawText("You Win!", SCREEN_WIDTH / 2 - MeasureText("You Win!", 48) / 2, SCREEN_HEIGHT / 2, 48, WHITE);
        }
    }
    else
    {
        // Lose text
        DrawText("You Lose...", SCREEN_WIDTH / 2 - MeasureText("You Lose...", 48), SCREEN_HEIGHT / 2, 48, WHITE);
    }

    EndDrawing();
}

void Game::Unload()
{

}

void Game::UpdateDrawFrame()
{
    Update();
    Draw();
}

void Game::EraseShot(Shot shot)
{
    shots.erase(std::find(shots.begin(), shots.end(), shot));
    std::cout << "Erased shot at position " << shot.position.x << ", " << shot.position.y << std::endl; // debug
}

void Game::EraseShot(std::vector<Shot>::iterator iter)
{
    shots.erase(iter);
    std::cout << "Erased shot at position " << shots[iter - shots.begin()].position.x << ", " << shots[iter - shots.begin()].position.y << std::endl; // debug
}
