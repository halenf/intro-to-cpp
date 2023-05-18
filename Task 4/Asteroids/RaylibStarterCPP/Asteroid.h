#pragma once
#include "GameObject.h"
enum AsteroidType { Small, Medium, Large };

class Asteroid : public GameObject {
public:
	Asteroid()
	{ };
	~Asteroid()
	{ };

	float radius;
	AsteroidType type;
	float health;
	bool active = true;
	float respawnTimer;
public:
	
};
