#pragma once

#include "Point2D.h"

class Powerup;
class Player;

class Room
{
public:
	Room();
	~Room();

	void SetPosition(Point2D position);
	void SetType(int type);

	int GetType();

	void Draw();
	void DrawDescription();

	bool ExecuteCommand(int command, Player* pPlayer);

private:
	bool Pickup(Player* pPlayer);

private:
	Point2D m_mapPosition;
	int m_type;

	Powerup* m_powerup;
};

