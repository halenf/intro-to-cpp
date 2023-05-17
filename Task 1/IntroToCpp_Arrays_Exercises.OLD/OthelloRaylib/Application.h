#pragma once

#include <vector>
#include <string>

typedef std::vector<std::vector<char>> OthelloBoardState;

class Application
{
public:
	Application();
	~Application();

	void Run();

	static bool IsValidMove(char player, int col, int row, const OthelloBoardState& grid);
	static OthelloBoardState MakeMove(char player, int col, int row, OthelloBoardState grid);

protected:

	void Load();
	void Unload();

	void Update(float dt);
	void Draw();

	bool AnyValidMoves(char player, const OthelloBoardState& grid);
	void CheckWinState();

protected:

	int m_tileSize = 64;

	OthelloBoardState m_board;
	char m_player = 'B';

	std::string m_endGameMessage = "";

private:
};
