#pragma once
#include "GameObject.h"
class Shot : public GameObject {
public:
	Shot()
	{ };
	~Shot()
	{ };

	float radius;
	int lifeTime;

public:
	friend bool operator== (const Shot& s1, const Shot& s2)
	{
		return (s1.position.x == s2.position.x && s1.position.y == s2.position.y && s1.lifeTime == s2.lifeTime);
	}
};