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
	ifstream file("highscores.dat");
	if (file.is_open()) { file.close(); return true; }
	else { file.close(); return false; }
}

void FileWriter::CreateHighscoreFile()
{
	if (!CheckHighscoreFileExists())
	{
		ofstream file("highscores.dat", ios::binary);

		string name = "GUY";
		int score = 0;

		for (int i = 0; i < HIGHSCORE_COUNT; i++)
		{
			file.write(name.c_str(), sizeof(char) * HIGHSCORE_NAME_MAX);
			file.write((char*)&score, sizeof(int));
		}
		file.close();
	}
}

string FileWriter::SaveHighscore(string name, int score)
{
	for (int i = 0; i < HIGHSCORE_COUNT; i++)
	{
		if (score > highScores[i].score)
		{
			for (int j = HIGHSCORE_COUNT - 1; j > i; j--)
			{
				highScores[j] = highScores[j - 1];
			}
			
			highScores[i].name = name;
			highScores[i].score = score;
			WriteHighscores();
			LoadHighscores();
			return "Score saved!";
		}
	}
	return "Your score does not beat the existing highscores...";
}

void FileWriter::LoadHighscores()
{
	highScores.clear();
	ifstream file("highscores.dat", ios::binary);

	for (int i = 0; i < HIGHSCORE_COUNT; i++)
	{
		char* name = new char[4];
		int score;

		file.read(name, sizeof(char) * HIGHSCORE_NAME_MAX);
		file.read((char*)&score, sizeof(int));

		name[3] = '\0';

		HighScore highScore;
		highScore.name = name;
		highScore.score = score;
		highScores.push_back(highScore);
	}
	file.close();

	// debug
	/*
	cout << "Highscores: ";
	for (vector<HighScore>::iterator it = highScores.begin(); it < highScores.end(); ++it)
		cout << it->name << " : " << it->score << ", ";
	cout << endl;
	*/
}

void FileWriter::WriteHighscores()
{
	remove("highscores.dat");
	ofstream file("highscores.dat", ios::binary);

	for (int i = 0; i < HIGHSCORE_COUNT; i++)
	{
		const char* name = highScores[i].name.c_str();
		int score = highScores[i].score;

		file.write(name, sizeof(char) * HIGHSCORE_NAME_MAX);
		file.write((char*)&score, sizeof(int));
	}

	file.close();
}
