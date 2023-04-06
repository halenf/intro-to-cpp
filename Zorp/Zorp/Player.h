#pragma once

#include "Point2D.h"
#include <vector>

class Powerup;

class Player
{
public:
	Player();
	Player(int x, int y);
	Player(Point2D position);
	~Player();

	void AddPowerup(Powerup* pPowerup);

	void SetPosition(Point2D position);

	Point2D GetPosition();

	void Draw();

	bool ExecuteCommand(int command);

private:
	Point2D m_mapPosition;

	std::vector<Powerup*> m_powerups;

	int m_healthPoints;
	int m_attackPoints;
	int m_defencePoints;
};

