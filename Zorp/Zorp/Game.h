#pragma once

#include "GameDefines.h"
#include "Room.h"
#include "Player.h"

class Game
{
public:
	Game();
	~Game();

public:
	bool Startup();
	void Update();
	void Draw();
	
	bool IsGameOver();

private:
	bool EnableVirtualTerminal();
	void InitialiseMap();

	void DrawWelcomeMessage();
	void DrawMap();
	void DrawVaildDirections();

	int GetCommand();

private:
	bool m_gameOver;
	Room m_map[MAZE_HEIGHT][MAZE_WIDTH];
	Player m_player;
};

