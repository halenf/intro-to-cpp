#include "GameObject.h"
#include "raymath.h"
#include <iostream>
#include <string>

GameObject::GameObject()
{ }

GameObject::~GameObject()
{ }


void GameObject::SetPosition(float x, float y)
{
	position = { x, y };
}

void GameObject::SetVelocity(float x, float y)
{
	velocity = { x, y };
}

void GameObject::SetVelocity(Vector2 velocity)
{
	this->velocity = velocity;
}

void GameObject::AddVelocity(float x, float y)
{
	velocity.x += x;
	velocity.y += y;
}

void GameObject::AddVelocity(Vector2 velocity)
{
	this->velocity.x += (velocity.x);
	this->velocity.y += (velocity.y);
}
