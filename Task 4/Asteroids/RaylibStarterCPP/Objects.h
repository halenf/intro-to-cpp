#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>

typedef struct Player {
	Vector2 position;
	Vector2 velocity;
	float acceleration;
	float rotation;
	Vector3 collider;
	Color colour;
} Player;

typedef struct Shot {
	Vector2 position;
	Vector2 velocity;
	float radius;
	float rotation;
	int lifeTime;
	Color colour;

public:
	friend bool operator== (const Shot& s1, const Shot& s2)
	{
		return (s1.position.x == s2.position.x && s1.position.y == s2.position.y);
	}
} Shot;

enum AsteroidType { Small, Medium, Large };

typedef struct Asteroid {
	Vector2 position;
	Vector2 velocity;
	float radius;
	AsteroidType type;
	float health;
	bool active;
	Color colour;
} Asteroid;

enum PowerupType { Heal, Boost, Bomb };

typedef struct Powerup {
	Vector2 position;
	PowerupType type;
	float radius;
	bool active;
	Color colour;
} Powerup;
