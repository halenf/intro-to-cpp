#include "DataFile.h"
#include <fstream>
#include <iostream>
using namespace std;

DataFile::DataFile()
{ }

DataFile::~DataFile()
{ }

// Creates a new record from the parameters and adds it to a vector to be saved with DataFile::Save later
void DataFile::AddRecord(string imageFilename, string name, int age)
{
	Image i = LoadImage(imageFilename.c_str());

	Record* r = new Record;
	r->image = i;
	r->name = name;
	r->age = age;

	newRecords.push_back(r);
}

// Reads the int at the start of the database representing the number of records it contains
// and stores it in recordCount
// Saves the position of the first record to the recordIndexes array
// to be used for loading with Database::Load
void DataFile::GetDatabaseInfo(string filename)
{
	ifstream infile(filename, ios::binary);
	infile.read((char*)&recordCount, sizeof(int)); // Set the record count
	cout << "Record Count: " << recordCount << endl;
	recordIndexes = new int[recordCount];
	recordIndexes[0] = infile.tellg();

	// Intialise variables for record reading
	int nameSize = 0;
	int ageSize = 0;
	int width = 0, height = 0, format = 0, imageSize = 0;

	infile.close();
}

// Saves the current records and any new ones to a new database
void DataFile::Save(string readFilename, string writeFilename)
{
	ifstream infile(readFilename, ios::binary);

	// Put all the existing records into a vector
	vector<DataFile::Record*> records;
	for (int i = 0; i < recordCount; i++)
	{
		records.push_back(Load(readFilename, i));
	}
	infile.close();

	for (int i = 0; i < newRecords.size(); i++)
	{
		records.push_back(newRecords[i]); // Add any new records to the vector of existing records
	}

	ofstream outfile(writeFilename + ".dat", ios::binary);
	int newRecordCount = records.size();
	outfile.write((char*)&newRecordCount, sizeof(int)); // Write the new record size to the new database

	// Write all the record data to the new database
	for (int i = 0; i < recordCount; i++)
	{		
		Color* imgdata = GetImageData(records[i]->image);
				
		int imageSize = sizeof(Color) * records[i]->image.width * records[i]->image.height;
		int nameSize = records[i]->name.length();
		int ageSize = sizeof(int);

		outfile.write((char*)&records[i]->image.width, sizeof(int));
		outfile.write((char*)&records[i]->image.height, sizeof(int));
		
		outfile.write((char*)&nameSize, sizeof(int));
		outfile.write((char*)&ageSize, sizeof(int));

		outfile.write((char*)imgdata, imageSize);
		outfile.write((char*)records[i]->name.c_str(), nameSize);
		outfile.write((char*)&records[i]->age, ageSize);
	}

	outfile.close();
}

// Returns a record at a specified index from a specified database
DataFile::Record* DataFile::Load(string filename, int index)
{
	ifstream infile(filename, ios::binary);

	infile.seekg(recordIndexes[index], ios::beg); // Set the reader position to the record location

	// Intialise variables for record reading
	int nameSize = 0;
	int ageSize = 0;
	int width = 0, height = 0, format = 0, imageSize = 0;

	infile.read((char*)&width, sizeof(int)); // Read and store the image width from the .dat
	infile.read((char*)&height, sizeof(int)); // Read and store the image height from the .dat

	infile.read((char*)&nameSize, sizeof(int)); // Read and store the length of the name from the .dat
	infile.read((char*)&ageSize, sizeof(int)); // Read and store the length of the age from the .dat

	imageSize = sizeof(Color) * width * height; // Set the filesize of the image
	char* imgdata = new char[imageSize]; // Initialise variable for image data
	infile.read(imgdata, imageSize); // Read and store the image data from the .dat
	Image img = LoadImageEx((Color*)imgdata, width, height); // Create a raylib image file using the image data

	char* name = new char[nameSize]; // Initialise variable for name
	int age = 0; // Initialise variable for age

	infile.read((char*)name, nameSize); // Read and store the name from the .dat
	infile.read((char*)&age, ageSize); // Read and store the age from the .dat

	// Save the information to a new record and then return it
	Record* r = new Record();
	r->image = img;
	r->name = string(name);
	r->age = age;

	delete[] imgdata;
	delete[] name;

	if (index <= recordCount)
	{
		recordIndexes[index + 1] = infile.tellg(); // Save the current position (the start of the next record) to the recordIndexes array
	}

	infile.close();

	return r;
}
