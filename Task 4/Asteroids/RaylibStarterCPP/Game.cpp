#include "Game.h"
#include "raylib.h"
#include "raymath.h"
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <random>
#include <string>

Game::Game()
{ }

Game::~Game()
{ }

void Game::Init()
{
    // Set the rng seed
    srand(time(0));
    
    // Used for determining position and velocity of game objects
    float posX, posY;
    float velX, velY;

    // Game state
    gameOver = false;
    win = false;
    pause = false;

    // Set player values
    score = 0;
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
        InitAsteroid(i);
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
            /// Player logic
            // Rotate player
            if (IsKeyDown(KEY_A)) player.rotation -= PLAYER_ROTATION_SPEED;
            if (IsKeyDown(KEY_D)) player.rotation += PLAYER_ROTATION_SPEED;
            player.velocity = { sin(player.rotation * DEG2RAD) * PLAYER_SPEED, -cos(player.rotation * DEG2RAD) * PLAYER_SPEED };

            // Accelerate player
            if (IsKeyDown(KEY_W))
            {
                if (player.acceleration < 1) player.acceleration += playerAcceleration;
            }
            else if (IsKeyDown(KEY_S))
            {
                if (player.acceleration > 0) player.acceleration -= playerAcceleration;
                else if (player.acceleration < 0) player.acceleration = 0;
            }
            else
            {
                if (player.acceleration > 0) player.acceleration -= DRAG_CONSTANT;
                else if (player.acceleration < 0) player.acceleration = 0;
            }

            if (IsKeyDown(KEY_LEFT_SHIFT)) // Slows the player down if they hold left shift
            {
                player.position.x += player.velocity.x * player.acceleration / 2;
                player.position.y += player.velocity.y * player.acceleration / 2;
            }
            else
            {
                player.position.x += player.velocity.x * player.acceleration;
                player.position.y += player.velocity.y * player.acceleration;
            }

            // Screen wrap
            if (player.position.x > SCREEN_WIDTH + shipHeight) player.position.x = -shipHeight;
            else if (player.position.x < -shipHeight) player.position.x = SCREEN_WIDTH + shipHeight;
            if (player.position.y > SCREEN_HEIGHT + shipHeight) player.position.y = -shipHeight;
            else if (player.position.y < -shipHeight) player.position.y = SCREEN_HEIGHT + shipHeight;

            /// Shooting logic
            // Shot init
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

            // Update position and erase shot if it hits the edge of the screen
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

            // Erase shot if it exists for the set lifetime
            for (int i = 0; i < shots.size(); i++)
            {
                if (shots[i].lifeTime >= SHOT_LIFE_TIME)
                {
                    EraseShot(shots[i]);
                    break;
                }
                shots[i].lifeTime++;
            }

            /// Asteroid logic
            for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
            {
                // Update position and screen wrap
                asteroids[i].position.x += asteroids[i].velocity.x;
                asteroids[i].position.y += asteroids[i].velocity.y;

                if (asteroids[i].position.x > SCREEN_WIDTH + asteroids[i].radius) asteroids[i].position.x = -asteroids[i].radius;
                else if (asteroids[i].position.x < -asteroids[i].radius) asteroids[i].position.x = SCREEN_WIDTH + asteroids[i].radius;
                if (asteroids[i].position.y > SCREEN_HEIGHT + asteroids[i].radius) asteroids[i].position.y = -asteroids[i].radius;
                else if (asteroids[i].position.y < -asteroids[i].radius) asteroids[i].position.y = SCREEN_HEIGHT + asteroids[i].radius;

                // Deactivate if no health
                if (asteroids[i].health <= 0)
                {
                    asteroids[i].active = false;
                }

                // Respawn inactive asteroid if respawn timer reaches the set respawn time
                // Otherwise increase timer
                if (!asteroids[i].active)
                {
                    if (asteroids[i].respawnTimer >= ASTEROID_RESPAWN_TIME)
                    {
                        asteroids[i].active = true;
                    }
                    else
                    {
                        asteroids[i].respawnTimer += GetFrameTime();
                    }
                }
            }

            /// Collisions
            // Set player collider
            player.collider = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };

            // Check if the player has hit an asteroid and lost the game
            for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
            {
                if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, asteroids[i].position, asteroids[i].radius) && asteroids[i].active) gameOver = true;
            }

            // Check shot-asteroid collision
            for (int i = 0; i < shots.size(); i++)
            {
                for (int j = 0; j < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; j++)
                {
                    if (asteroids[j].active && CheckCollisionCircles(shots[i].position, shots[i].radius, asteroids[j].position, asteroids[j].radius))
                    {
                        std::cout << "Shot at position " << shots[i].position.x << ", " << shots[i].position.y << " collided with asteroid at position " << asteroids[j].position.x << ", " << asteroids[j].position.y << std::endl; // debug
                        
                        asteroids[j].health -= SHOT_DAMAGE; // Do damage to asteroid
                        if (!asteroids[j].health <= 0)// Apply a force to the asteroid if it wasn't destroyed
                        {
                            // Create a vector representing the direction between the position of the two objects
                            Vector2 direction = Vector2Subtract(asteroids[j].position, shots[i].position);
                            // Calculate the angle between the bullet and the direction
                            float vectorAngle = acos(Vector2DotProduct(shots[i].velocity, direction) / Vector2Length(shots[i].velocity) / Vector2Length(direction)) * RAD2DEG;

                            // If the angle between the bullet's velocity and the direction of the bullet to the asteroid's centre is low,
                            // it means the bullet collided 'dead-on', or it was a direct collision.
                            // If the angle is high, it means the bullet only 'glanced' the asteroid.
                            // The multiplier changes the amount of momentum the bullet transfers to the asteroid based on this
                            float multiplier = (90.0f - vectorAngle) / 90.0f / ((int)asteroids[j].type + 1);
                            asteroids[j].AddVelocity(shots[i].velocity, multiplier);
                        }
                        else
                        {
                            score += (int)(asteroids[i].type + 1) * 100; // Add score
                        }
                        EraseShot(shots[i]); // Erase the shot
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // Runs if the player has lost
        if (IsKeyPressed(KEY_ENTER))
        {
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

        // Draw score
        std::string sScore = "Score: " + std::to_string(score);
        DrawText(sScore.c_str(), SCREEN_WIDTH - 162, 0, 24, WHITE);

        // Pause text
        if (pause)
        {
            DrawText("Paused", (SCREEN_WIDTH - MeasureText("Paused", 48)) / 2, SCREEN_HEIGHT / 2 - 24, 48, WHITE);
        }
    }
    else
    {
        // Game over text
        DrawText("Game Over!", (SCREEN_WIDTH - MeasureText("Game Over!", 48)) / 2, SCREEN_HEIGHT / 2 - 24, 48, WHITE);
        DrawText("Final Score:", (SCREEN_WIDTH - MeasureText("Final Score:", 24)) / 2, SCREEN_HEIGHT / 2 + 40, 24, WHITE);
        std::string sScore = std::to_string(score);
        DrawText(sScore.c_str(), (SCREEN_WIDTH - MeasureText(sScore.c_str(), 24)) / 2, SCREEN_HEIGHT / 2 + 72, 24, WHITE);
        DrawText("Press ENTER to play again or ESC to exit.", (SCREEN_WIDTH - MeasureText("Press ENTER to play again or ESC to exit.", 18)) / 2, SCREEN_HEIGHT / 2 + 96, 18, WHITE);
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

void Game::InitAsteroid(int i)
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

    float posX = randPosX[GetRandomValue(0, 1)];
    float posY = randPosY[GetRandomValue(0, 1)];

    // Randomly set velocity based on asteroid type and asteroid base speed
    // Both cannot be 0 (that would make the asteroid stationary
    float velX = 0;
    float velY = 0;

    while (velX == 0 || velY == 0)
    {
        velX = (rand() % (int)(ASTEROID_BASE_SPEED * 2) - (int)ASTEROID_BASE_SPEED) / (float)(type + 1) / 100;
        velY = (rand() % (int)(ASTEROID_BASE_SPEED * 2) - (int)ASTEROID_BASE_SPEED) / (float)(type + 1) / 100;
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
    asteroids[i].respawnTimer = 0;
    asteroids[i].active = true;
}
