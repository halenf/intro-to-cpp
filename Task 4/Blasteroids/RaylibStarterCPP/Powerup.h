#pragma once
#include "GameObject.h"

enum PowerupType { Heal, Boost, Bomb };

class Player;

class Powerup : public GameObject {
public:
	Powerup()
	{ };
	~Powerup()
	{ };

	PowerupType type;
	float radius;
	bool active;
	float respawnTimer;

	void ActivatePowerup(Player &player);

private:
	void HealPlayer(Player &player, int health);
	void BoostPlayer(Player &player, float time, float speedMultiplier);
	void BombPlayer(Player &player, int bulletCount);

};