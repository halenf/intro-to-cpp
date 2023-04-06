#include "Powerup.h"
#include "GameDefines.h"
#include <iostream>

Powerup::Powerup(const char name[30], float health, float attack, float defence) : m_healthMultiplier{health}, m_attackMultiplier{attack}, m_defenceMultiplier{defence}
{ 
	for (size_t i = 0; i < 30; i++)
	{
		m_name[i] = name[i];
	}
}

Powerup::~Powerup()
{ }

char* Powerup::GetName()
{
	return m_name;
}

float Powerup::GetHealthMultiplier()
{
	return m_healthMultiplier;
}

float Powerup::GetAttackMultiplier()
{
	return m_attackMultiplier;
}

float Powerup::GetDefenceMultiplier()
{
	return m_defenceMultiplier;
}

bool Powerup::Compare(const Powerup* p1, const Powerup* p2)
{
	return (strcmp(p1->m_name, p2->m_name) < 0) ? true : false;
}
