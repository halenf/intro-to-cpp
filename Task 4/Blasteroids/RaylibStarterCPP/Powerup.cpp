#include "Powerup.h"
#include "Player.h"

void Powerup::ActivatePowerup(Player &player)
{
	switch (type) {
	case Heal:
		HealPlayer(player, 1);
		break;
	case Boost:
		BoostPlayer(player, 2.0f, 1.5f);
		break;
	case Bomb:
		BombPlayer(player, 20);
		break;
	}
}

void Powerup::HealPlayer(Player &player, int health)
{
	if (player.health < PLAYER_MAX_HEALTH) player.health += health;
}

void Powerup::BoostPlayer(Player &player, float time, float speedMultiplier)
{
	player.boostTimer = time;
	player.boostMultiplier = speedMultiplier;
}

void Powerup::BombPlayer(Player &player, int bulletCount)
{
	player.shouldBomb = true;
	player.bombCount = bulletCount;
}