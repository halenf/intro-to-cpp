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
	std::string SaveHighscore(std::string name, int score); // Saves a highscore to 'highscore.dat' if it beats a highscore
	
	void LoadHighscores(); // Loads the highscores from 'highscores.dat'

	struct HighScore
	{
	public:
		std::string name;
		int score;
	};

	std::vector<HighScore> highScores;

private:
	bool CheckHighscoreFileExists(); // Returns if the 'highscores.dat' file exists
	void WriteHighscores(); // Writes all of the current highscores in 'highScores' to the database

};
