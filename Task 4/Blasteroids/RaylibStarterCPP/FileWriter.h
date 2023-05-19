#pragma once
#include <vector>
#include <string>
#include "GameDefines.h"

class FileWriter
{
public:
	FileWriter();
	~FileWriter();

	void CreateHighscoreFile(); // Creates the 'highscores.dat' file if it doesnt exist
	void SaveHighscore(); // Saves a highscore to 'highscore.dat' if it 
	void LoadHighscores(); // Loads the highscores from 'highscores.dat'

	struct HighScore
	{
	public:
		std::string name;
		int score;
	};

	std::vector<HighScore> highScores;

	struct SortByScore
	{
		inline bool operator() (const HighScore& highscore1, const HighScore& highscore2)
		{
			return (highscore1.score > highscore2.score);
		}
	};

private:
	bool CheckHighscoreFileExists(); // Returns if the 'highscores.dat' file exists

};
