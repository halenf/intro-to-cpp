#include "Game.h"
#include "raymath.h"

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <math.h>
#include <random>
#include <string>
#include <cctype>

using namespace std;

Game::Game()
{
    gameState = Start;
}

Game::~Game()
{ }

void Game::Init()
{
    switch (gameState) {
    case Start:
        // Set the rng seed
        srand(time(0));

        // Set static values
        shipHeight = (PLAYER_SIZE / 2) / tanf(20 * DEG2RAD);
        player.colour = RED;

        // Setup highscoreWriter
        highscoreWriter.CreateHighscoreFile();

        break;

    case Playing:
        // Highscore saving
        highscoreWriter.highScores.clear();
        name[0] = '\0';
        charCount = 0;
        frameCounter = 0;
        scoreSaved = false;
        displayText = "";

        // Initialise player
        score = 0;
        player.position = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - shipHeight / 2 };
        player.velocity = { 0, 0 };
        player.acceleration = 0;
        player.rotation = 0;
        player.health = PLAYER_MAX_HEALTH;
        player.collider = { player.position.x + (float)sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - (float)cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };
        player.boostMultiplier = 1;
        player.boostTimer = 0;
        player.shouldBomb = false;

        // Initialise shots
        shots.clear();
        shots.reserve(20);

        // Initialise asteroids
        int arraySet[3] = { 0, 0, 0 };
        copy(begin(arraySet), end(arraySet), asteroidCount); // Reset asteroidCount
        for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
        {
            InitAsteroid(i);
        }

        // Initialise powerups
        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            InitPowerup(i);
            powerups[i].active = false;
        }
        break;
    }
}

void Game::Update()
{
    switch (gameState) {
    case Start:
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            gameState = Playing;
            Init();
        }
        break;
    }

    case Playing:
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

            // Checking if the player should be boosting
            if (player.boostMultiplier != 1)
            {
                if (player.boostTimer <= 0)
                {
                    player.boostMultiplier = 1;
                }
                else
                {
                    player.boostTimer -= GetFrameTime();
                }
            }

            // Calculate player acceleration
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

            // Slows the player down if they hold left shift
            float lowGear;
            if (IsKeyDown(KEY_LEFT_SHIFT))
            {
                lowGear = 0.5f;
            }
            else
            {
                lowGear = 1;
            }

            // Update player position
            player.position.x += player.velocity.x * player.boostMultiplier * player.acceleration * lowGear;
            player.position.y += player.velocity.y * player.boostMultiplier * player.acceleration * lowGear;

            // Screen wrap
            if (player.position.x > SCREEN_WIDTH + shipHeight) player.position.x = -shipHeight;
            else if (player.position.x < -shipHeight) player.position.x = SCREEN_WIDTH + shipHeight;
            if (player.position.y > SCREEN_HEIGHT + shipHeight) player.position.y = -shipHeight;
            else if (player.position.y < -shipHeight) player.position.y = SCREEN_HEIGHT + shipHeight;

            // Player health
            if (player.health <= 0)
            {
                gameState = GameOver;
            }

            /// Shooting logic
            // Shot init
            if (IsKeyPressed(KEY_SPACE))
            {
                InitShot(0);
            }

            // Bomb powerup
            if (player.shouldBomb)
            {
                float angle = 360.0f / (float)player.bombCount;

                for (int i = 0; i < player.bombCount; i++)
                {
                    InitShot((i + 1) * angle);
                }

                player.shouldBomb = false;
                player.bombCount = 0;
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

                // Respawn inactive asteroid if respawn timer reaches the set respawn time
                // Otherwise increase timer
                if (!asteroids[i].active)
                {
                    if (asteroids[i].respawnTimer <= 0)
                    {
                        InitAsteroid(i);
                    }
                    else
                    {
                        asteroids[i].respawnTimer -= GetFrameTime();
                    }
                }
            }

            // Powerup logic
            for (int i = 0; i < MAX_POWERUPS; i++)
            {
                // Powerup spawning
                if (!powerups[i].active)
                {
                    if (powerups[i].respawnTimer <= 0)
                    {
                        InitPowerup(i);
                    }
                    else
                    {
                        powerups[i].respawnTimer -= GetFrameTime();
                    }
                }
            }

            /// Collisions
            // Set player collider
            player.collider = { player.position.x + sin(player.rotation * DEG2RAD) * (shipHeight / 2.5f), player.position.y - cos(player.rotation * DEG2RAD) * (shipHeight / 2.5f), 12 };

            // Check if the player has hit an asteroid
            for (int i = 0; i < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; i++)
            {
                if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, asteroids[i].position, asteroids[i].radius) && asteroids[i].active)
                {
                    player.health -= 1;
                    asteroids[i].respawnTimer = ASTEROID_RESPAWN_TIME;
                    asteroids[i].active = false;
                }
            }

            // Check if the player has hit a powerup
            for (int i = 0; i < MAX_POWERUPS; i++)
            {
                if (CheckCollisionCircles({ player.collider.x, player.collider.y }, player.collider.z, powerups[i].position, powerups[i].radius) && powerups[i].active)
                {
                    powerups[i].ActivatePowerup(player);
                    powerups[i].respawnTimer = GetRandomValue(MIN_POWERUP_SPAWN_TIME, MAX_POWERUP_SPAWN_TIME) / 100.0f;
                    powerups[i].active = false;
                }
            }

            // Check shot-asteroid collision
            for (int i = 0; i < shots.size(); i++)
            {
                for (int j = 0; j < MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID; j++)
                {
                    if (asteroids[j].active && CheckCollisionCircles(shots[i].position, shots[i].radius, asteroids[j].position, asteroids[j].radius))
                    {
                        //cout << "Shot at position " << shots[i].position.x << ", " << shots[i].position.y << " collided with asteroid at position " << asteroids[j].position.x << ", " << asteroids[j].position.y << endl; // debug
                        asteroids[j].health -= SHOT_DAMAGE; // Do damage to asteroid
                        if (asteroids[j].health > 0) // Apply a force to the asteroid if it wasn't destroyed
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
                        else // If it was destroyed, increase the score and deactivate it
                        {
                            score += (int)(asteroids[j].type + 1) * 100;
                            asteroids[j].respawnTimer = ASTEROID_RESPAWN_TIME;
                            asteroids[j].active = false;
                        }
                        EraseShot(shots[i]); // Erase the shot
                        break;
                    }
                }
            }
        }
        break;
    }

    case GameOver:
    {
        /// Runs if the player has lost
        // Load the highscores
        if (highscoreWriter.highScores.empty())
        {
            highscoreWriter.LoadHighscores();
        }

        // Player can only interact with the highscore-saving text box if they haven't yet saved their score
        if (!scoreSaved)
        {
            // Submit score
            if (IsKeyPressed(KEY_ENTER) && strlen(name) != 0)
            {
                displayText = highscoreWriter.SaveHighscore(name, score); // Sets the display text based on if the player's highscore was saved
                scoreSaved = true;
            }

            // Get keyboard key pressed
            int key = GetKeyPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                if (key == 32)
                {
                    break;
                }
                if (charCount < HIGHSCORE_NAME_MAX)
                {
                    name[charCount] = toupper((char)key);
                    name[charCount + 1] = '\0'; // Add null terminator at the end of the C-string.
                    charCount++;

                    key = GetKeyPressed();  // Check next character in the queue
                }
                else
                {
                    key = 0;
                }
            }

            // Delete character from name
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                charCount--;
                if (charCount < 0) charCount = 0;
                name[charCount] = '\0';
            }
        }
        // Restart game
        else if (IsKeyPressed(KEY_ENTER))
        {
            gameState = Playing;
            Init();
        }

        break;
    }
    }
}

void Game::Draw()
{
    BeginDrawing();

    ClearBackground(BLACK);

    switch (gameState) {
    case Start:
    {
        // Title animation
        for (int i = 0; i < TITLE_EFFECT_COUNT; i++)
        {
            Color colour = { 255, 255, 255, (float)(255 / (i + 1) - 150 / (i + 1)) };
            float xOffset = 20 * cos(titleAngle - i);
            float yOffset = 20 * sin(titleAngle - i);
            DrawText("Blasteroids", (SCREEN_WIDTH - MeasureText("Blasteroids", 72)) / 2 + xOffset, SCREEN_HEIGHT / 2 - 36 + yOffset, 72, colour);
        }

        float xOffset = 32 * cos(2.5f * titleAngle + 2) * sin(8 - titleAngle);
        float yOffset = 32 * cos(0.56f * titleAngle) * sin(titleAngle);

        DrawText("Blasteroids", (SCREEN_WIDTH - MeasureText("Blasteroids", 72)) / 2 + xOffset, SCREEN_HEIGHT / 2 - 36 + yOffset, 72, WHITE);

        titleAngle += GetFrameTime();

        DrawText("Press ENTER to play", (SCREEN_WIDTH - MeasureText("Press ENTER to play", 18)) / 2, SCREEN_HEIGHT / 2 + 96, 18, WHITE);
        break;
    }

    case Playing:
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

        // Draw powerups
        for (int i = 0; i < MAX_POWERUPS; i++)
        {
            if (powerups[i].active) DrawCircle(powerups[i].position.x, powerups[i].position.y, powerups[i].radius, powerups[i].colour);
        }

        // Draw FPS
        DrawText(to_string(GetFPS()).c_str(), 0, 0, 24, WHITE);

        // Draw score
        string sScore = "Score: " + to_string(score);
        DrawText(sScore.c_str(), SCREEN_WIDTH - 162, 0, 24, WHITE);

        // Draw player health
        DrawPlayerHealth(0, SCREEN_HEIGHT - 24);

        // Pause text
        if (pause)
        {
            DrawText("Paused", (SCREEN_WIDTH - MeasureText("Paused", 48)) / 2, SCREEN_HEIGHT / 2 - 24, 48, WHITE);
        }
        break;
    }

    case GameOver:
    {
        /// Game over text
        DrawText("Game Over!", (SCREEN_WIDTH - MeasureText("Game Over!", 48)) / 2, SCREEN_HEIGHT / 2 - 120, 48, WHITE);
        DrawText("Final Score:", (SCREEN_WIDTH - MeasureText("Final Score:", 24)) / 2, SCREEN_HEIGHT / 2 - 56, 24, WHITE);
        string sScore = to_string(score);
        DrawText(sScore.c_str(), (SCREEN_WIDTH - MeasureText(sScore.c_str(), 36)) / 2, SCREEN_HEIGHT / 2 - 24, 36, WHITE);
        if (scoreSaved)
            DrawText("Press ENTER to play again or ESC to exit.", (SCREEN_WIDTH - MeasureText("Press ENTER to play again or ESC to exit.", 18)) / 2, SCREEN_HEIGHT / 2 + 20, 18, WHITE);
        else
            DrawText("Press ENTER to save your score or ESC to exit.", (SCREEN_WIDTH - MeasureText("Press ENTER to save your score or ESC to exit.", 18)) / 2, SCREEN_HEIGHT / 2 + 20, 18, WHITE);
        DrawText(displayText.c_str(), (SCREEN_WIDTH - MeasureText(displayText.c_str(), 18)) / 2, SCREEN_HEIGHT / 2 + 100, 18, WHITE);

        if (!highscoreWriter.highScores.empty())
        {
            for (int i = 0; i < HIGHSCORE_COUNT; i++)
            {
                string name = highscoreWriter.highScores[i].name;
                string score = to_string(highscoreWriter.highScores[i].score);
                string highscore = to_string(i + 1) + ". " + name + ": " + score;
                DrawText(highscore.c_str(), (SCREEN_WIDTH - MeasureText(highscore.c_str(), 20)) / 2, SCREEN_HEIGHT / 2 + 120 + (i * 20), 20, WHITE);
            }
        }

        /// Draw text input field
        float boxX = SCREEN_WIDTH / 2 - 30;
        float boxY = SCREEN_HEIGHT / 2 + 60;
        float fontSize = 24;

        Color colour;
        if (scoreSaved) { colour = GRAY; }
        else { colour = WHITE; }

        DrawRectangleLines(boxX, boxY, 60, fontSize + 2, colour);
        DrawText(name, boxX + 2, boxY, fontSize, colour);

        if (charCount < HIGHSCORE_NAME_MAX && !scoreSaved)
        {
            // Draw blinking underscore char
            if (((frameCounter / 20) % 2) == 0)
            {
                DrawText("_", (int)boxX + 6 + MeasureText(name, fontSize), (int)boxY + 2, fontSize, WHITE);
            }
            frameCounter++;
        }

        break;
    }
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
    shots.erase(find(shots.begin(), shots.end(), shot));
    //cout << "Erased shot at position " << shot.position.x << ", " << shot.position.y << endl; // debug
}

void Game::EraseShot(vector<Shot>::iterator iter)
{
    shots.erase(iter);
    //cout << "Erased shot at position " << shots[iter - shots.begin()].position.x << ", " << shots[iter - shots.begin()].position.y << endl; // debug
}

void Game::InitShot(float angle)
{
    if (shots.size() == shots.capacity())
    {
        EraseShot(shots.begin());
    }
    Shot shot;
    shot.position = { player.position.x + sin((player.rotation + angle) * DEG2RAD) * shipHeight, player.position.y - cos((player.rotation + angle) * DEG2RAD) * shipHeight };
    shot.velocity = { 1.5f * sin((player.rotation + angle) * DEG2RAD) * PLAYER_SPEED, 1.5f * -cos((player.rotation + angle) * DEG2RAD) * PLAYER_SPEED };
    shot.rotation = player.rotation + angle;
    shot.radius = SHOT_SIZE;
    shot.colour = YELLOW;
    shot.lifeTime = 0;
    shots.push_back(shot);
}

void Game::GenerateRandomPositionAtDistance(float &posX, float &posY, int distance)
{
    // Used for ensuring that a gameObject can't spawn within a certain distance of the player
    // If the possible spawn location for the object is not within the range of the screen, it instead overflows to the other side of the screen
    float playerX = player.position.x;
    if (playerX + distance > SCREEN_WIDTH)
    {
        playerX -= SCREEN_WIDTH;
    }
    if (playerX - distance < 0)
    {
        playerX += SCREEN_WIDTH;
    }

    float playerY = player.position.y;
    if (playerY + distance > SCREEN_HEIGHT)
    {
        playerY -= SCREEN_HEIGHT;
    }
    if (playerY - distance < 0)
    {
        playerY += SCREEN_HEIGHT;
    }

    // Sets ranges for position that are at least 'distance' units away from the player
    float randPosX[2] = { GetRandomValue(0, playerX - distance), GetRandomValue(playerX + distance, SCREEN_WIDTH) };
    float randPosY[2] = { GetRandomValue(0, playerY - distance), GetRandomValue(playerY + distance, SCREEN_HEIGHT) };

    posX = randPosX[GetRandomValue(0, 1)];
    posY = randPosY[GetRandomValue(0, 1)];
}

void Game::InitAsteroid(int i)
{
    // Generate random asteroid type
    // Ensure an asteroid of a certain type is not created if there is already a maximum of that type
    // Will only set type on game restart
    int type;
    if (asteroids[i].active)
    {
        type = GetRandomValue(0, 2);
        while (asteroidCount[type] >= maxAsteroid[type])
        {
            type = GetRandomValue(0, 2);
        }
        asteroidCount[type] += 1;
    }
    else
    {
        type = (int)asteroids[i].type;
    }
    
    // Set the spawn position
    float posX = 0, posY = 0;
    GenerateRandomPositionAtDistance(posX, posY, 100);

    // Randomly set velocity based on asteroid type and asteroid base speed
    // Both cannot be 0 (that would make the asteroid stationary
    float velX = 0, velY = 0;

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

void Game::InitPowerup(int i)
{
    // Set the powerup type, max 1 of each type
    int type = i;

    // Set the spawn position
    float posX = 0, posY = 0;
    while (posX < 50 || posY < 50 || posX > SCREEN_WIDTH - 50 || posY > SCREEN_HEIGHT - 50)
    {
        GenerateRandomPositionAtDistance(posX, posY, 100);
    }

    // Set radius
    float radius = POWERUP_SIZE;

    // Set colour based on type
    Color colour = powerupColours[type];

    powerups[i].position = { posX, posY };
    powerups[i].velocity = { 0,0 };
    powerups[i].radius = radius;
    powerups[i].type = (PowerupType)type;
    powerups[i].colour = colour;
    powerups[i].respawnTimer = GetRandomValue(MIN_POWERUP_SPAWN_TIME, MAX_POWERUP_SPAWN_TIME) / 100.0f;
    powerups[i].active = true;
}

void Game::DrawPlayerHealth(float x, float y)
{
    DrawRectangle(x, y, PLAYER_MAX_HEALTH * HEALTHBAR_SIZE, 24, RED);
    DrawRectangle(x, y, player.health * HEALTHBAR_SIZE, 24, GREEN);
}
