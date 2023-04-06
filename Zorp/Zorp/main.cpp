#include <iostream>
#include "Game.h"

void main()
{	
	Game game;

	if (game.Startup() == false)
	{
		return;
	}

	// Game loop
	while (!game.IsGameOver())
	{
		game.Draw();

		game.Update();
	}
	// End of game loop

	// Exit program
	std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
	std::cout << std::endl << INDENT << "Press 'Enter' to exit the program." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
}
