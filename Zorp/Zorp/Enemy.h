#pragma once

#include "Point2D.h"

class Enemy
{
public:
	Enemy() : m_healthPoints{ 20 }, m_attackPoints{ 10 }, m_defencePoints{ 5 } {};
	~Enemy() {};

	void SetPosition(const Point2D& position) { m_mapPosition = position; }
	Point2D GetPosition() { return m_mapPosition;  }

	int GetHP() { return m_healthPoints; }
	int GetAT() { return m_attackPoints; }
	int GetDF() { return m_defencePoints; }

	bool IsAlive();

	void OnAttacked(int attackPoints);

private:
	Point2D m_mapPosition;

	int m_healthPoints;
	int m_attackPoints;
	int m_defencePoints;
};

