#include "Game.h"
#include <iostream>
#include <windows.h>
#include <random>
#include <time.h>

Game::Game() : m_gameOver{ false }
{ }

Game::~Game()
{ }

bool Game::Startup()
{
	if (EnableVirtualTerminal() == false)
	{
		std::cout << "The virtual terminal processing mode could not be activated." << std::endl;
		std::cout << "Press 'Enter' to exit." << std::endl;
		std::cin.get();
		return false;
	}

	InitialiseMap();

	m_player.SetPosition(Point2D{ 0, 0 });

	DrawWelcomeMessage();

	return true;
}

void Game::Update()
{
	// Get the player's current position
	Point2D playerPos = m_player.GetPosition();

	// Check if the player is on the exit tile, and if they are, they win
	if (m_map[playerPos.y][playerPos.x].GetType() == EXIT)
	{
		m_gameOver = true;
		return;
	}

	// Get user input
	int command = GetCommand();

	// Check if the user wants to quit
	if (command == QUIT)
	{
		m_gameOver = true;
		return;
	}

	// Check if the command is a player command
	if (m_player.ExecuteCommand(command))
	{
		return;
	}

	// If the command isn't a player command, it has to be a room command
	m_map[playerPos.y][playerPos.x].ExecuteCommand(command, &m_player);
}

void Game::Draw()
{
	Point2D playerPos = m_player.GetPosition();

	// List directions available to player
	DrawVaildDirections();

	// draw the description of the current room
	m_map[playerPos.y][playerPos.x].DrawDescription();

	// redraw map
	DrawMap();

	// Draw player onto map
	m_player.Draw();
}

bool Game::IsGameOver()
{
	return m_gameOver;
}

bool Game::EnableVirtualTerminal()
{
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode))
	{
		return false;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode))
	{
		return false;
	}

	return true;
}

void Game::InitialiseMap()
{
	srand(time(nullptr)); // Set random starting number based on current time

	// Set the contents of each room in the map
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			// Set room type
			int type = rand() % (MAX_RANDOM_TYPE * 2);
			if (type < MAX_RANDOM_TYPE)
			{
				// if room type is 'TREASURE', set it to contain a random powerup type
				if (type == TREASURE)
				{
					type = rand() % 3 + TREASURE_HP;
				}
				m_map[y][x].SetType(type);

			}
			else
			{
				m_map[y][x].SetType(EMPTY);
			}

			m_map[y][x].SetPosition(Point2D{ x, y }); // Set room position
		}
	}
	m_map[0][0].SetType(ENTRANCE); // Set the first room to be the entrance
	m_map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1].SetType(EXIT); // Set the last room to be the exit
}

void Game::DrawWelcomeMessage()
{
	// Introduction
	std::cout << TITLE << MAGENTA << "Weclome to ZORP!" << RESET_COLOR << std::endl;
	std::cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << std::endl;
	std::cout << INDENT << "It is definitely not related to any other text-based adventure game.\n\n";
}

void Game::DrawMap()
{
	Point2D position = { 0, 0 };

	// reset draw colours
	std::cout << RESET_COLOR;
	for (position.y = 0; position.y < MAZE_HEIGHT; position.y++)
	{
		for (position.x = 0; position.x < MAZE_WIDTH; position.x++)
		{
			m_map[position.y][position.x].Draw();
		}
		std::cout << std::endl;
	}
}

void Game::DrawVaildDirections()
{
	Point2D position = m_player.GetPosition();

	// Reset draw colours
	std::cout << RESET_COLOR;
	// Jump to correct console location
	std::cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
	// List the directions the player can take
	std::cout << INDENT << "You can see paths leading to the " <<
		((position.x > 0) ? "west, " : "") <<
		((position.x < MAZE_WIDTH - 1) ? "east, " : "") <<
		((position.y > 0) ? "north, " : "") <<
		((position.y < MAZE_HEIGHT - 1) ? "south " : "") << std::endl;
}

int Game::GetCommand()
{
	// C-string of 50 characters max
	char input[50] = "\0";

	// move cursor to correct position
	std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	// Clear existing text
	std::cout << CSI << "4M";
	// Insert 4 blank lines to ensure that the inventory output remains correct
	std::cout << CSI << "4L";

	std::cout << INDENT << "What now?";

	// move cursor to position for player to enter input
	std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

	// clear the input buffer, ready for player input
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());

	std::cin >> input;
	std::cout << RESET_COLOR;

	bool bMove = false;
	bool bPickup = false;
	while (input)
	{
		// checks if the player has decided to move
		if (strcmp(input, "move") == 0)
		{
			bMove = true;
		}
		// checks the direction the player chose to move if they chose to move
		else if (bMove = true)
		{
			if (strcmp(input, "north") == 0)
				return NORTH;
			if (strcmp(input, "south") == 0)
				return SOUTH;
			if (strcmp(input, "east") == 0)
				return EAST;
			if (strcmp(input, "west") == 0)
				return WEST;
		}

		// checks for other actions
		if (strcmp(input, "look") == 0)
		{
			return LOOK;
		}

		if (strcmp(input, "fight") == 0)
		{
			return FIGHT;
		}

		if (strcmp(input, "exit") == 0)
		{
			return QUIT;
		}

		if (strcmp(input, "pick") == 0)
		{
			bPickup = true;
		}
		else if (bPickup == true)
		{
			if (strcmp(input, "up") == 0)
			{
				return PICKUP;
			}
		}

		// peek at next character in 'input' to see if the loop has read through all the player's inputs yet
		char next = std::wcin.peek();
		if (next == '\n' || next == EOF)
			break;
		std::cin >> input;
	}
	return 0;
}
