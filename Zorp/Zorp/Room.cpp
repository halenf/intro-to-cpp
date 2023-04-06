#include "Room.h"
#include "GameDefines.h"
#include "Player.h"
#include "Powerup.h"
#include <iostream>

static const char itemNames[15][30] =
{
	"indifference", "invisibility", "invulnerability", "incontinence", "improbability", "impatience", "indecision", "inspiration", "independence",
	"incurability", "integration", "invocation", "inferno", "indigestion", "inoculation"
};

Room::Room() : m_type{ EMPTY }, m_mapPosition{ 0, 0 }, m_powerup{ nullptr }
{ }

Room::~Room()
{
	if (m_powerup != nullptr)
	{
		delete m_powerup;
	}
}

void Room::SetPosition(Point2D position)
{
	m_mapPosition = position;
}

void Room::SetType(int type)
{
	m_type = type;

	// if the room type isn't 'TREASURE' then it is not given an item
	if (!(m_type == TREASURE_HP || m_type == TREASURE_AT || m_type == TREASURE))
	{
		return;
	}
	// if the room already has a powerup then it is not overriden
	if (m_powerup != nullptr)
	{
		return;
	}

	// Pick a random item descriptor
	int item = rand() % 15;
	char name[30] = "";

	float HP = 1;
	float AT = 1;
	float DF = 1;

	// Set item stats based on powerup type
	switch (type)
	{
	case TREASURE_HP:
		strcpy_s(name, "potion of ");
		HP = 1.1f;
		break;
	case TREASURE_AT:
		strcpy_s(name, "sword of ");
		AT = 1.1f;
		break;
	case TREASURE_DF:
		strcpy_s(name, "shield of ");
		DF = 1.1f;
		break;
	}

	// Append item name to the string
	strncat_s(name, itemNames[item], 30);
	// Set room item to the new item
	m_powerup = new Powerup(name, HP, AT, DF);
}

int Room::GetType()
{
	return m_type;
}

void Room::Draw()
{
	// find the console output position
	int outX = INDENT_X + (6 * m_mapPosition.x) + 1;
	int outY = MAP_Y + m_mapPosition.y;

	// move cursor to the position
	std::cout << CSI << outY << ";" << outX << "H";

	// draw the room
	switch (m_type)
	{
	case EMPTY:
		std::cout << "[ " << GREEN << "\xb0" << RESET_COLOR << " ] ";
		break;
	case ENEMY:
		std::cout << "[ " << RED << "\x94" << RESET_COLOR << " ] ";
		break;
	case TREASURE_HP:
	case TREASURE_AT:
	case TREASURE_DF:
		std::cout << "[ " << YELLOW << "$" << RESET_COLOR << " ] ";
		break;
	case FOOD:
		std::cout << "[ " << BLUE << "\xcf" << RESET_COLOR << " ] ";
		break;
	case ENTRANCE:
		std::cout << "[ " << WHITE << "\x9d" << RESET_COLOR << " ] ";
		break;
	case EXIT:
		std::cout << "[ " << WHITE << "\xFE" << RESET_COLOR << " ] ";
		break;
	}
}

void Room::DrawDescription()
{
	// Prep screen for output
	std::cout << RESET_COLOR;
	std::cout << CSI << ROOM_DESC_Y << ";" << 0 << "H" << "A" << CSI << "4M" << CSI << "4L" << std::endl;

	// Write description of current room
	switch (m_type)
	{
	case EMPTY:
		std::cout << INDENT << "You are in an empty meadow. There is nothing of interest here." << std::endl;
		break;
	case ENEMY:
		std::cout << INDENT << "BEWARE. An enemy is approaching." << std::endl;
		break;
	case TREASURE_HP:
	case TREASURE_AT:
	case TREASURE_DF:
		std::cout << INDENT << "There appears to be some treasure here. Perhaps you should investigate further." << std::endl;
		break;
	case FOOD:
		std::cout << INDENT << "At last! You collect some food to sustain you on your journey." << std::endl;
		break;
	case ENTRANCE:
		std::cout << INDENT << "The entrance you used to enter this maze is blocked. There is no going back." << std::endl;
		break;
	case EXIT:
		std::cout << INDENT << "Despite all odds, you made it to the exit. Congratulations." << std::endl;
		break;
	}
}

bool Room::ExecuteCommand(int command, Player* pPlayer)
{
	std::cout << EXTRA_OUTPUT_POS;
	switch (command)
	{
	case LOOK:
		// Text displayed for 'TREASURE' rooms
		if (m_type == TREASURE_HP || m_type == TREASURE_AT || m_type == TREASURE_DF)
		{
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There is some treasure here. It looks light enough to carry." << std::endl;
		}
		// Text displayed for 'EMPTY' rooms
		else
		{
			std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
				"You look around, but see nothing worth mentioning" << std::endl;
		}
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		return true;
	case FIGHT:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
			"You could try to fight, but you don't have a weapon" << std::endl;
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		return true;
	case PICKUP:
		return Pickup(pPlayer);
	default:
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR <<
			"You try, but you just can't do it." << std::endl;
		std::cout << INDENT << "Press 'Enter' to continue.";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cin.get();
		break;
	}
	return false;
}

bool Room::Pickup(Player* pPlayer)
{
	if (m_powerup == nullptr)
	{
		std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "There's nothing here to pick up." << std::endl;
		return true;
	}

	std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You pick up the " << m_powerup->GetName() << std::endl;

	// Add the powerup to the player's inventory
	pPlayer->AddPowerup(m_powerup);

	// Remove the powerup from the room, but don't delete as it is now in the player's inventory
	m_powerup = nullptr;

	// Change the room to empty
	m_type = EMPTY;

	std::cout << INDENT << "Press 'ENTER' to continue.";
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
	return true;
}
