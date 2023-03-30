#include <iostream>
#include <vector>

/*
You are required to implement the IsValidMove method for an 'Othello' game.
ALL TESTS CASES MUST PASS.

You are required to implement the MakeMove method for an Othello Game
ALL TEST CASES MUST PASS

*/

typedef std::vector<std::vector<char>> OthelloBoardState;

bool IsValidMove(char player, int col, int row, const OthelloBoardState& grid);
OthelloBoardState MakeMove(char player, int col, int row, OthelloBoardState grid);


int main(int argc, char** argv)
{
    OthelloBoardState state1 =
    {//   0    1    2    3    4    5    6    7
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 0
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 1
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 2
        {' ', ' ', ' ', 'W', 'B', ' ', ' ', ' '}, // 3
        {' ', ' ', ' ', 'B', 'W', ' ', ' ', ' '}, // 4
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 5
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 6
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}  // 7
    };

    auto ShouldBe = [](auto result, auto expected) {
        std::cout << (result == expected ? "\033[32m [PASS]" : "\033[31m [FAIL]") << "\033[0m" << std::endl;
    };


    ShouldBe(IsValidMove('B', 5, 4, state1), true);  // true
    ShouldBe(IsValidMove('B', 2, 4, state1), false); // false
    ShouldBe(IsValidMove('B', 3, 2, state1), true);  // true
    ShouldBe(IsValidMove('B', 3, 5, state1), false); // false
    ShouldBe(IsValidMove('B', 5, 5, state1), false); // false
    ShouldBe(IsValidMove('W', 5, 4, state1), false); // false
    ShouldBe(IsValidMove('W', 2, 4, state1), true);  // true
    ShouldBe(IsValidMove('W', 3, 2, state1), false); // false
    ShouldBe(IsValidMove('W', 3, 5, state1), true);  // true
    ShouldBe(IsValidMove('W', 5, 5, state1), false); // false

    OthelloBoardState state2 =
    {//   0    1    2    3    4    5    6    7
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 0
        {' ', ' ', 'B', ' ', ' ', ' ', ' ', ' '}, // 1
        {' ', ' ', 'B', 'W', ' ', ' ', ' ', ' '}, // 2
        {' ', ' ', 'B', 'W', 'B', ' ', ' ', ' '}, // 3
        {' ', ' ', 'W', 'W', 'W', ' ', ' ', ' '}, // 4
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 5
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}, // 6
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}  // 7
    };

    ShouldBe(IsValidMove('B', 2, 5, state2), true);  // true
    ShouldBe(IsValidMove('B', 3, 5, state2), false); // false
    ShouldBe(IsValidMove('B', 4, 5, state2), true);  // true
    ShouldBe(IsValidMove('B', 3, 1, state2), false); // false
    ShouldBe(IsValidMove('B', 4, 2, state2), true);  // true
    ShouldBe(IsValidMove('B', 4, 1, state2), true);  // true

    ShouldBe(IsValidMove('W', 2, 5, state2), false); // false
    ShouldBe(IsValidMove('W', 2, 0, state2), true);  // true
    ShouldBe(IsValidMove('W', 3, 1, state2), false); // false

    return 0;
}

bool IsValidMove(char player, int col, int row, const OthelloBoardState& grid)
{
    // Task 1:
    // this method should return true if the 'player'
    // can be placed on the grid at the specified row/col location.

    return false;
}

OthelloBoardState MakeMove(char player, int col, int row, OthelloBoardState grid)
{
    // if the move was not valid, just return the origional...
    if (IsValidMove(player, col, row, grid) == false)
        return grid;

    // Task 2:
    // Implement the make move method
    // The method should alter the 'grid' variable, and return it as a copy
    // taht represets the new board state

    return grid;
}