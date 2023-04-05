#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

// currently on 'Part 6: Functions'

// Define constants
const char* ESC = "\x1b";
const char* CSI = "\x1b[";

const char* TITLE = "\x1b[5;20H";
const char* INDENT = "\x1b[5C";
const char* RED = "\x1b[91m";
const char* GREEN = "\x1b[92m";
const char* YELLOW = "\x1b[93m";
const char* BLUE = "\x1b[94m";
const char* MAGENTA = "\x1b[95m";
const char* CYAN = "\x1b[96m";
const char* WHITE = "\x1b[97m";
const char* RESET_COLOR = "\x1b[0m";

// Room constants
const int EMPTY = 0;
const int ENEMY = 1;
const int TREASURE = 2;
const int FOOD = 3;
const int ENTRANCE = 4;
const int EXIT = 5;

const int MAX_RANDOM_TYPE = FOOD + 1; // Used for generating the room contents randomly

const int MAZE_WIDTH = 10; // Width of the map
const int MAZE_HEIGHT = 6; // Height of the map

// Map constants
const int INDENT_X = 5; // How many spaces to indent all text
const int ROOM_DESC_Y = 8; // Console line used for room descriptions
const int MOVEMENT_DESC_Y = 9; // uhhhh
const int MAP_Y = 13; // First line where the map is drawn

// Player constants
const char* EXTRA_OUTPUT_POS = "\x1b[25;6H";
const int PLAYER_INPUT_X = 30; // Character column where movement input is typed
const int PLAYER_INPUT_Y = 23; // Console line where movement input is typed
const int WEST = 4; // Input values for movement directions
const int EAST = 6; 
const int NORTH = 8;
const int SOUTH = 2;

const int LOOK = 9; // Input value for look
const int FIGHT = 10; // Input value for fight

// Struct declarations
struct Point2D
{
	int x, y;
};

// Define functions
bool EnableVirtualTerminal();
void Initialise(int map[MAZE_HEIGHT][MAZE_WIDTH]);
void DrawWelcomeMessage();
void DrawRoom(int map[MAZE_HEIGHT][MAZE_WIDTH], Point2D position);
void DrawMap(int map[MAZE_HEIGHT][MAZE_WIDTH]);
void DrawRoomDescription(int roomType);
void DrawPlayer(Point2D position);
void DrawValidDirections(Point2D position);
int GetCommand();

void main()
{	
	// 2D rooms array
	int map[MAZE_HEIGHT][MAZE_WIDTH];

	// Variables
	bool gameOver = false; // Whether or not the main game loop should be running
	Point2D player = { 0, 0 };

	if (EnableVirtualTerminal() == false)
	{
		std::cout << "The virtual terminal processing mode could not be activated." << std::endl;
		std::cout << "Press 'Enter' to exit." << std::endl;
		std::cin.get();
		return;
	}

	Initialise(map);

	DrawWelcomeMessage();

	DrawMap(map);

	// Game loop
	while (!gameOver)
	{
		DrawRoomDescription(map[player.y][player.x]);

		DrawPlayer(player);
		
		if (map[player.y][player.x] == EXIT)
		{
			gameOver = true;
			continue;
		}

		DrawValidDirections(player);
		int command = GetCommand();

		// before updating the player position, redraw the old room character over the old position
		DrawRoom(map, player);

		// Now update the player's position on the map based on the value of 'direction'
		switch (command) {
			case EAST:
				if (player.x < MAZE_WIDTH - 1)
					player.x++;
				break;
			case WEST:
				if (player.x > 0)
					player.x--;
				break;
			case NORTH:
				if (player.y > 0)
					player.y--;
				break;
			case SOUTH:
				if (player.y < MAZE_HEIGHT - 1)
					player.y++;
			case LOOK:
				DrawPlayer(player);
				std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You look around, but see nothing of mention." << std::endl;
				std::cout << INDENT << "Press ENTER to continue.";
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
				std::cin.get();
				break;
			case FIGHT:
				DrawPlayer(player);
				std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You could try to fight, but you don't have a weapon." << std::endl;
				std::cout << INDENT << "Press 'Enter' to continue.";
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
				std::cin.get();
			default:
				// invalid input, do nothing, go back to the top of the loop and ask again
				DrawPlayer(player);
				std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You try, but you just can't do it." << std::endl;
				std::cout << INDENT << "Press 'Enter' to continue.";
				std::cin.clear();
				std::cin.ignore(std::cin.rdbuf()->in_avail());
				std::cin.get();
				break;
		}
	}
	// End of game loop

	// Exit program
	std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	std::cout << std::endl << INDENT << "Press 'Enter' to exit the program." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
}

bool EnableVirtualTerminal()
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

void Initialise(int map[MAZE_HEIGHT][MAZE_WIDTH])
{
	srand(time(nullptr)); // Set random starting number based on current time
	// Set the contents of each room in the map
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			int type = rand() % (MAX_RANDOM_TYPE * 2);
			if (type < MAX_RANDOM_TYPE)
			{
				map[y][x] = type;
			}
			else
			{
				map[y][x] = EMPTY;
			}
		}
	}
	map[0][0] = ENTRANCE; // Set the first room to be the entrance
	map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT; // Set the last room to be the exit
}

void DrawWelcomeMessage()
{
	// Introduction
	std::cout << TITLE << MAGENTA << "Weclome to ZORP!" << RESET_COLOR << std::endl;
	std::cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << std::endl;
	std::cout << INDENT << "It is definitely not related to any other text-based adventure game.\n\n";
}

void DrawRoom(int map[MAZE_HEIGHT][MAZE_WIDTH], Point2D position)
{
	// Find the console output position
	int outX = INDENT_X + (6 * position.x) + 1;
	int outY = MAP_Y + position.y;

	std::cout << CSI << outY << ";" << outX << "H"; // Jump to the correct map location
	switch (map[position.y][position.x]) // Different ASCII characters are drawn based on the room contents
	{
	case EMPTY:
		std::cout << "[ " << GREEN << "\xb0" << RESET_COLOR << " ] ";
		break;
	case ENEMY:
		std::cout << "[ " << RED << "\x94" << RESET_COLOR << " ] ";
		break;
	case TREASURE:
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

void DrawMap(int map[MAZE_HEIGHT][MAZE_WIDTH])
{
	// reset draw colours
	std::cout << RESET_COLOR;
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		std::cout << INDENT;
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			Point2D position = { x, y };
			DrawRoom(map, position);
		}
		std::cout << std::endl;
	}
}

void DrawRoomDescription(int roomType)
{
	// Prep screen for output
	std::cout << RESET_COLOR;
	std::cout << CSI << ROOM_DESC_Y << ";" << 0 << "H" << "A" << CSI << "4M" << CSI << "4L" << std::endl;

	// Write description of current room
	switch (roomType)
	{
	case EMPTY:
		std::cout << INDENT << "You are in an empty meadow. There is nothing of interest here." << std::endl;
		break;
	case ENEMY:
		std::cout << INDENT << "BEWARE. An enemy is approaching." << std::endl;
		break;
	case TREASURE:
		std::cout << INDENT << "Your journey has been rewarded. You have found some treasure." << std::endl;
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

void DrawPlayer(Point2D position)
{
	// Update player's position on map
	int outX = INDENT_X + (6 * position.x) + 3;
	int outY = MAP_Y + position.y;

	std::cout << CSI << outY << ";" << outX << "H"; // Delete the symbol current at the player's position
	std::cout << MAGENTA << "\x81"; // Draw the player's position on the map
}

void DrawValidDirections(Point2D position)
{
	// jump to the correct location
	std::cout << RESET_COLOR << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
	// List the directions the player can take
	std::cout << INDENT << "You can see paths leading to the " <<
		((position.x > 0) ? "west, " : "") <<
		((position.x < MAZE_WIDTH - 1) ? "east, " : "") <<
		((position.y > 0) ? "north, " : "") <<
		((position.y < MAZE_HEIGHT - 1) ? "south " : "") << std::endl;
}

int GetCommand()
{
	char input[50] = "\0";
	
	// move cursor to correct position
	std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	// Clear existing text
	std::wcout << CSI << "4M";

	std::cout << INDENT << "What now?";

	// move cursor to position for player to enter input
	std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

	// clear the input buffer, ready for player input
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());

	std::cin >> input;
	std::cout << RESET_COLOR;

	bool bMove = false;
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

		// peek at next character in 'input' to see if the loop has read through all the player's inputs yet
		char next = std::wcin.peek();
		if (next == '\n' || next == EOF)
			break;
		std::cin >> input;
	}

	return 0;
}
