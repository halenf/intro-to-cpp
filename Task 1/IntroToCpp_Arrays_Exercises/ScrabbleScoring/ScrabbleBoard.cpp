
#include "ScrabbleBoard.h"

#include <string>
#include <cstring>
#include <map>
#include <algorithm>

ScrabbleBoard::ScrabbleBoard(int rows, int cols) :
	ROWS(rows), COLS(cols)
{
	m_boardModifiers = new unsigned int[ROWS * COLS];
	m_boardCharacters = new char[ROWS * COLS];

	// Initialise board values
	for (int i = 0; i < ROWS * COLS; i++)
	{
		m_boardModifiers[i] = (int)EScoreMod::NO;
		m_boardCharacters[i] = ' ';
	}

	m_dictionary = new Dictionary("dictionary.txt");
}

ScrabbleBoard::~ScrabbleBoard()
{
	delete m_dictionary;
	delete[] m_boardModifiers;
	delete[] m_boardCharacters;
}

ScrabbleBoard::EScoreMod ScrabbleBoard::GetBoardModifier(int xIndex, int yIndex)
{
	int index = yIndex * ROWS + xIndex;
	return (EScoreMod)m_boardModifiers[index];
}

char ScrabbleBoard::GetBoardCharacter(int xIndex, int yIndex)
{
	int index = yIndex * ROWS + xIndex;
	return m_boardCharacters[index];
}

void ScrabbleBoard::SetBoardCharacter(int xIndex, int yIndex, char c)
{
	if (xIndex < 0 || yIndex < 0 || xIndex >= COLS || yIndex >= ROWS)
		return;

	int index = yIndex * ROWS + xIndex;
	m_boardCharacters[index] = c;
}

void ScrabbleBoard::SetBoardScoreModifiers(unsigned int* board)
{
	for (int i = 0; i < ROWS * COLS; i++)
		m_boardModifiers[i] = board[i];
}

void ScrabbleBoard::PlaceText(int xIndex, int yIndex, const char* text, EDirection dir)
{
	int score = CalculateScore(xIndex, yIndex, text, dir);
	m_totalScore += score;

	int xo = 0, yo = 0;
	switch (dir)
	{
	case EDirection::RIGHT: xo = 1; break;
	case EDirection::DOWN:  yo = 1; break;
	}

	int len = strlen(text);
	for (int i = 0; i < len; i++)
	{
		int xi = xIndex + (i * xo);
		int yi = yIndex + (i * yo);

		SetBoardCharacter(xi, yi, text[i]);
	}
}

unsigned int ScrabbleBoard::GetTotalScore()
{
	return m_totalScore;
}



unsigned int ScrabbleBoard::CalculateScore(int xIndex, int yIndex, const char* text, EDirection dir)
{
	// ========================================================================
	// Task 2
	/*
		The method GetBoardModifier will tell you if there are any score 
		modifier values on the board at a given location. 
		
		Modifiers are:
			NO: no score modification
			DL: Double Letter Score
			TL: Tripple Letter Score
			DW: Double Word Score
			TW: Tribble word score.

		When calculating the score, letter modifiers apply first to calculate 
		the word score. Word modifiers are calculated after the total word score.

		Letter scores:
			(1 point) -A, E, I, O, U, L, N, S, T, R
			(2 points)-D, G
			(3 points)-B, C, M, P
			(4 points)-F, H, V, W, Y
			(5 points)-K
			(8 points)-J, X
			(10 points)-Q, Z
	*/

	auto characterToPlace = text[0];
	auto characterOnBoard = GetBoardCharacter(xIndex, yIndex);
	auto modifier = GetBoardModifier(xIndex, yIndex);
	 
	int score = 1;

	return score;
}

bool ScrabbleBoard::CanPlaceText(int xIndex, int yIndex, const char* text, EDirection dir)
{
	// ========================================================================
	// Task 1:
	// 	   xIndex and yIndex represent the location of the first character for the "text"
	// 	   being placed on the board.
	// 
	// 	   This method should return true if the supplied "text" can be placed
	// 	   on the board.
	//		- return false if the text runs out of bounds
	// 	    - return false if the text overlaps other characters (unless the characters are the same)
	// 	    - return true if the text can be successfully placed on the board.
	// 
	// 	   use the method "GetBoardCharacter" to get the character on the board.
	// 
	// ========================================================================

	return true;
}
