#include "FileWriter.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

FileWriter::FileWriter()
{
	highScores.reserve(HIGHSCORE_COUNT);
}

FileWriter::~FileWriter() { }

bool FileWriter::CheckHighscoreFileExists()
{
	ifstream file("highscores/highscores.dat");
	if (file.is_open()) { file.close(); return true; }
	else { file.close(); return false; }
}

void FileWriter::CreateHighscoreFile()
{
	if (!CheckHighscoreFileExists())
	{
		ofstream file("highscores/highscores.dat");

		string name("Guy");
		char ch = 0 + '0';

		for (int i = 0; i < HIGHSCORE_COUNT; i++)
		{
			file.write((char*)name.size(), sizeof(name.size()));
			file.write(name.c_str(), name.size());
			cout << name.c_str() << endl; // debug
			file.write((char*)ch, sizeof(char));
		}
		file.close();
	}
}

void FileWriter::SaveHighscore()
{
	// Save to file
}

void FileWriter::LoadHighscores()
{
	highScores.clear();
	ifstream file("highscores/highscores.dat", ios::binary);
	for (int i = 0; i < HIGHSCORE_COUNT; i++)
	{
		string name;
		int size;
		int score = 0;
		
		
		file.read(&name[0], HIGHSCORE_NAME_MAX);
		file.read((char*)&score, sizeof(char));

		cout << "Name loaded: " << name << ", " << endl;
		cout << "Score loaded: " << score << ", " << endl;

		HighScore highScore;
		highScore.name = string(name);
		highScore.score = score;
		highScores.push_back(highScore);
	}
	file.close();

	//sort(highScores.begin(), highScores.end(), SortByScore());

	// debug
	cout << "highScores contains: ";
	for (std::vector<HighScore>::iterator it = highScores.begin(); it != highScores.end(); ++it)
		cout << it->name << ": " << it->score << ", ";
	cout << '\n';
}
