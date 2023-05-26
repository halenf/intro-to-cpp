#pragma once
#include "Asteroid.h"
#include "Player.h"
#include "Powerup.h"
#include "Shot.h"
#include "FileWriter.h"

enum GameState { Start, Playing, GameOver };

class Game
{
public:
	Game();
	~Game();

public:
	void Init();
	void Unload();	
	void UpdateDrawFrame();

private:
	// Private functions
	void Update();
	void Draw();

	// GameObject functions
	void EraseShot(Shot shot);
	void EraseShot(std::vector<Shot>::iterator iter);
	void InitShot(float angle);
	void GenerateRandomPositionAtDistance(float &posX, float &posY, int distance);
	void InitAsteroid(int i);
	void InitPowerup(int i);

	// Draw functions
	void DrawPlayerHealth(float x, float y);

	// Highscores
	FileWriter highscoreWriter;
	char name[HIGHSCORE_NAME_MAX + 1] = "\0";
	int charCount;
	bool scoreSaved;
	int frameCounter;
	std::string displayText;

	// Game states
	GameState gameState;
	bool pause;

	// Title screen effect
	float titleAngle;

	// Player info
	int score;
	float shipHeight;
	float playerAcceleration = PLAYER_ACCELERATION;

	// Game objects
	Player player;

	std::vector<Shot> shots;

	Asteroid asteroids[MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID];
	int maxAsteroid[3] = { MAX_SMALL_ASTEROID, MAX_MEDIUM_ASTEROID, MAX_LARGE_ASTEROID }; // Small , medium, large asteroids
	int asteroidCount[3]; // Small, medium, large asteroids

	Powerup powerups[MAX_POWERUPS];
	Color powerupColours[3] = { RED, BLUE, YELLOW };
	
};
