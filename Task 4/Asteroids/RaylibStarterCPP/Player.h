#pragma once
#include "GameObject.h"
class Player : public GameObject {
public:
	Player()
	{ };
	~Player()
	{ };

	float acceleration;
	Vector3 collider;
};