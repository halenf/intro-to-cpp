#pragma once
#include "GameDefines.h"

struct HighScore
{
public:
	char name[HIGHSCORE_NAME_MAX + 1];
	int score;
};