#pragma once

#define SCREEN_WIDTH 			800
#define SCREEN_HEIGHT 			800
#define FPS						60

#define DRAG_CONSTANT			0.02f
#define INELASTIC_CONSTANT		1.0f

#define PLAYER_SIZE				20.0f
#define PLAYER_SPEED			4.0f
#define PLAYER_MAX_SHOTS		10
#define PLAYER_ROTATION_SPEED	3.5f
#define PLAYER_ACCELERATION		0.04f;

#define SHOT_SIZE				2
#define SHOT_LIFE_TIME			50
#define SHOT_DAMAGE				1.0f

#define ASTEROID_BASE_HEALTH	1.0f
#define ASTEROID_BASE_SPEED		300.0f
#define ASTEROID_BASE_RADIUS	10.0f
#define MAX_LARGE_ASTEROID		4
#define MAX_MEDIUM_ASTEROID		8
#define MAX_SMALL_ASTEROID		12
#define ASTEROID_RESPAWN_TIME	3.0f

#define MAX_POWERUPS			3
#define MIN_POWERUP_SPAWN_TIME	5.0f
#define MAX_POWERUP_SPAWN_TIME	10.0f