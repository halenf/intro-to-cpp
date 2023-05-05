#pragma once
#include "GameDefines.h"
#include "Objects.h"

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

private:
	// Private functions
	bool Within100(int pos);

	// Game states
	bool gameOver;
	bool pause;
	bool win;

	// Player info
	float shipHeight;

	// Game objects
	Player player;
	Shot shot[PLAYER_MAX_SHOTS];
	Asteroid largeAsteroids[MAX_LARGE_ASTEROID];
	Asteroid mediumAsteroids[MAX_MEDIUM_ASTEROID];
	Asteroid smallAsteroids[MAX_SMALL_ASTEROID];
	Powerup powerups[MAX_POWERUPS];
};
