#include <iostream>
#include <Windows.h>
#include <random>
#include <time.h>

using namespace std;

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
const char* SAVE_CURSOR_POS = "\x1b[s";
const char* RESTORE_CURSOR_POS = "\x1b[u";

int main()
{	
	// Set output mode to handle virtual terminal sequences
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

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

	// Player constants
	const int INDENT_X = 5; // How many spaces to indent all text
	const int ROOM_DESC_Y = 8; // Console line used for room descriptions
	const int MAP_Y = 13; // First line where the map is drawn
	const int PLAYER_INPUT_X = 30; // Character column where movement input is typed
	const int PLAYER_INPUT_Y = 11; // Console line where movement input is typed
	const int WEST = 4; // Keypad inputs used to determine player movement input
	const int EAST = 6; // Could easily be replaced with 'wasd' keys
	const int NORTH = 8;
	const int SOUTH = 2;


	// 2D rooms array
	int rooms[MAZE_HEIGHT][MAZE_WIDTH];

	srand(time(nullptr)); // Set random starting number based on current time
	// Set the contents of each room in the map
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			int type = rand() % (MAX_RANDOM_TYPE * 2);
			if (type < MAX_RANDOM_TYPE)
			{
				rooms[y][x] = type;
			}
			else
			{
				rooms[y][x] = EMPTY;
			}
		}
	}
	rooms[0][0] = ENTRANCE; // Set the first room to be the entrance
	rooms[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT; // Set the last room to be the exit

	// Variables
	bool gameOver = false; // Whether or not the main game loop should be running
	int playerX = 0; // Player's X location on the map
	int playerY = 0; // Player's Y location on the map

	// Introduction
	cout << TITLE << MAGENTA << "Weclome to ZORP!" << RESET_COLOR << endl;
	cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << endl;
	cout << INDENT << "It is definitely not related to any other text-based adventure game.\n\n";

	cout << SAVE_CURSOR_POS; // Save cursor position

	// Game loop
	while (!gameOver)
	{
		// Draw the map
		cout << CSI << MAP_Y << ";" << 0 << "H";
		for (int y = 0; y < MAZE_HEIGHT; y++)
		{
			cout << INDENT;
			for (int x = 0; x < MAZE_WIDTH; x++)
			{
				switch (rooms[y][x]) // Different ASCII characters are drawn based on the room contents
				{
				case EMPTY:
					cout << "[ " << GREEN << "\xb0" << RESET_COLOR << " ] ";
					break;
				case ENEMY:
					cout << "[ " << RED << "\x94" << RESET_COLOR << " ] ";
					break;
				case TREASURE:
					cout << "[ " << YELLOW << "$" << RESET_COLOR << " ] ";
					break;
				case FOOD:
					cout << "[ " << WHITE << "\xcf" << RESET_COLOR << " ] ";
					break;
				case ENTRANCE:
					cout << "[ " << WHITE << "\x9d" << RESET_COLOR << " ] ";
					break;
				case EXIT:
					cout << "[ " << WHITE << "\xFE" << RESET_COLOR << " ] ";
					break;
				}
			}
			cout << endl;
		}
		
		// Prep screen for output: move cursor to start of the first question, then up 1 line, then delete and insert four lines
		cout << RESTORE_CURSOR_POS << CSI << "A" << CSI << "4M" << CSI << "4L" << endl;

		// Write description of current room
		switch (rooms[playerY][playerX])
		{
			case EMPTY:
				cout << INDENT << "You are in an empty meadow. There is nothing of interest here." << endl;
				break;
			case ENEMY:
				cout << INDENT << "BEWARE. An enemy is approaching." << endl;
				break;
			case TREASURE:
				cout << INDENT << "Your journey has been rewarded. You have found some treasure" << endl;
					break;
			case FOOD:
				cout << INDENT << "At last! You collect some food to sustain you on your journey." << endl;
					break;
			case ENTRANCE:
				cout << INDENT << "The entrance you used to enter this maze is blocked.There is no going back." << endl;
					break;
			case EXIT:
				cout << INDENT << "Despite all odds, you made it to the exit. Congratulations." << endl;
					gameOver = true;
				continue;
		}

		// List the directions the player can take
		cout << INDENT << "You can see paths leading to the " <<
			((playerX > 0) ? "west, " : "") <<
			((playerX < MAZE_WIDTH - 1) ? "east, " : "") <<
			((playerY > 0) ? "north, " : "") <<
			((playerY < MAZE_HEIGHT - 1) ? "south " : "") << endl;

		cout << INDENT << "Where to now?";

		// Update player's position on map
		int x = INDENT_X + (6 * playerX) + 3;
		int y = MAP_Y + playerY;

		cout << CSI << y << ";" << x << "H"; // Delete the symbol current at the player's position
		cout << MAGENTA << "\x81"; // Draw the player's position on the map

		// move cursor to position for player to enter input
		cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

		// clear the input buffer, ready for player input
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());

		int direction = 0;
		cin >> direction;
		cout << RESET_COLOR;

		if (cin.fail())
			continue; // go back to the top of the game loop and ask again

		// before updating the player position, redraw the old room character over the old position
		cout << CSI << y << ";" << x << "H";
		switch (rooms[playerY][playerX])
		{
			case EMPTY:
				cout << GREEN << "\xb0" << RESET_COLOR;
				break;
			case ENEMY:
				cout << RED << "\x94" << RESET_COLOR;
				break;
			case TREASURE:
				cout << YELLOW << "$" << RESET_COLOR;
				break;
			case FOOD:
				cout << WHITE << "\xcf" << RESET_COLOR;
				break;
			case ENTRANCE:
				cout << WHITE << "\x9d" << RESET_COLOR;
				break;
			case EXIT:
				cout << WHITE << "\xFE" << RESET_COLOR;
				break;
		}

		// Now update the player's position on the map based on the value of 'direction'
		switch (direction) {
			case EAST:
				if (playerX < MAZE_WIDTH - 1)
					playerX++;
				break;
			case WEST:
				if (playerX > 0)
					playerX--;
				break;
			case NORTH:
				if (playerY > 0)
					playerY--;
				break;
			case SOUTH:
				if (playerY < MAZE_HEIGHT - 1)
					playerY++;
			default:
				break;
		}
	}

	// Exit program
	cout << endl << INDENT << "Press 'Enter' to exit the program." << endl;
	cin.clear();
	cin.ignore(cin.rdbuf()->in_avail());
	cin.get();
	return 0;
}
