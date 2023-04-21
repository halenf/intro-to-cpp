#pragma once

#include "raylib.h"
#include <string>
#include <vector>

using namespace std;

class DataFile
{
public:
	struct Record {
		Image image; // The NPC's face/photo
		string name; // The NPC's name
		int age; // The NPC's age
	};

private:
	int recordCount; // Stores the number of records in the database
	int recordIndexes[255]; // Stores the location of all the records in the database
	std::vector<Record*> newRecords; // Contains new records waiting to be saved to the database

public:
	DataFile();
	~DataFile();

	void AddRecord(string imageFilename, string name, int age); // Creates a new record from the parameters and adds it to a vector to be saved with DataFile::Save later
	void ClearNewRecords() { newRecords.clear(); }; // Clears newRecords

	int RecordCount() { return recordCount; }; // Returns the number of records in the database

	void GetDatabaseInfo(string filename); // Stores the number of records in the database

	void Save(string readFilename, string writeFilename); // Saves the current records and any new ones to a new database
	DataFile::Record* Load(string filename, int index); // Returns a record at a specified index from a specified database

};

