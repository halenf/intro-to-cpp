#pragma once
#include "raylib.h"

class GameObject {
public:
	GameObject();
	~GameObject();

	void SetPosition(float x, float y);
	void SetVelocity(float x, float y);
	void SetVelocity(Vector2 velocity);
	void AddVelocity(float x, float y);
	void AddVelocity(Vector2 velocity);

	Vector2 position;
	Vector2 velocity;
	float rotation;
	Color colour;
	
public:

};
