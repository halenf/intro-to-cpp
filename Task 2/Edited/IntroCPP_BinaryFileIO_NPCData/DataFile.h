#pragma once

#include "raylib.h"
#include <string>

using namespace std;

class DataFile
{
public:
	struct Record {
		Image image;
		string name;
		int age;
	};

private:
	int recordCount;



public:
	DataFile();
	~DataFile();

	void AddRecord(string imageFilename, string name, int age);

	int GetRecordCount() { return recordCount; };

	//void Save(string filename);
	DataFile::Record* Load(string filename, int index);

};

