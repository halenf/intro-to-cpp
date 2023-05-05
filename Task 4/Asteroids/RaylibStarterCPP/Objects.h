#pragma once
#include "raylib.h"

typedef struct Player {
	Vector2 position;
	Vector2 speed;
	float acceleration;
	float rotation;
	Vector3 collider;
	Color colour;
} Player;

typedef struct Shot {
	Vector2 position;
	Vector2 speed;
	float radius;
	float rotation;
	int lifeSpawn;
	bool active;
	Color colour;
} Shot;

typedef struct Asteroid {
	Vector2 position;
	Vector2 speed;
	float radius;
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
};
