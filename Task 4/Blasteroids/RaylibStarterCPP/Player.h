#pragma once
#include "GameObject.h"
class Player : public GameObject {
public:
	Player()
	{ };
	~Player()
	{ };

	float acceleration;
	float health;
	Vector3 collider;

	// Powerups
	float boostMultiplier;
	float boostTimer;
	bool shouldBomb;
	int bombCount;
};