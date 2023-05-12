#pragma once
#include "GameObject.h"

enum PowerupType { Heal, Boost, Bomb };

class Powerup : public GameObject {
public:
	Powerup()
	{ };
	~Powerup()
	{ };

	PowerupType type;
	float radius;
	bool active;
};