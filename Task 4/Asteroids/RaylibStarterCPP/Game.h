#pragma once
#include "GameDefines.h"
#include "Objects.h"
#include <iostream>
#include <vector>

class Game
{
public:
	Game();
	~Game();

public:
	void Init();
	void Update();
	void Draw();
	void Unload();	
	void UpdateDrawFrame();

private:
	// Private functions
	void EraseShot(Shot shot);
	void EraseShot(std::vector<Shot>::iterator iter);

	// Game states
	bool gameOver;
	bool pause;
	bool win;

	// Player info
	float shipHeight;
	float playerAcceleration = PLAYER_ACCELERATION;

	// Game objects
	Player player;
	std::vector<Shot> shots;
	Asteroid asteroids[MAX_LARGE_ASTEROID + MAX_MEDIUM_ASTEROID + MAX_SMALL_ASTEROID];
	Powerup powerups[MAX_POWERUPS];
	int maxAsteroid[3] = { MAX_SMALL_ASTEROID, MAX_MEDIUM_ASTEROID, MAX_LARGE_ASTEROID }; // Small , medium, large asteroids
	int asteroidCount[3]; // Small, medium, large asteroids
};
