/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "DataFile.h"
#include "ctype.h"
#include <iostream>

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    string currentDatabase;
    cout << "Insert name of database to be loaded." << endl;
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    cin >> currentDatabase;

    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "NPC Data-Checker");

    DataFile data;
    int currentRecordIdx = 0;
    
    // Attempts to load the database from the 'currentDatabase' filename
    data.GetDatabaseInfo(currentDatabase);

    DataFile::Record* currentRecord = data.Load(currentDatabase, currentRecordIdx);
    Texture2D recordTexture = LoadTextureFromImage(currentRecord->image);
    char name[255]; // Create a char array

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // When the left arrow key is pressed, iterate backwards through the NPC database
        if (IsKeyPressed(KEY_LEFT))
        {
            currentRecordIdx--;
            if (currentRecordIdx < 0) // currentRecordIdx can only represent a number indicating the index of an NPC in the database
            {
                currentRecordIdx = 0;
            }
            else // Only loads the NPC data if the value of currentRecordIdx changes
            {
                currentRecord = data.Load(currentDatabase, currentRecordIdx);
                recordTexture = LoadTextureFromImage(currentRecord->image);
            }
        }

        // Same as above but for iterating forwards through the database
        if (IsKeyPressed(KEY_RIGHT))
        {
            currentRecordIdx++;
            if (currentRecordIdx > data.RecordCount() - 1)
            {
                currentRecordIdx = data.RecordCount() - 1;
            }
            else
            {
                currentRecord = data.Load(currentDatabase, currentRecordIdx);
                recordTexture = LoadTextureFromImage(currentRecord->image);
            }
        }

        // Lets you insert record details to be saved to the database
        if (IsKeyPressed(KEY_A))
        {
            // Declare empty variables for calling 'AddRecord'
            string inputFile;
            string inputName;
            int inputAge;

            cout << "Please insert the filename of the image." << endl;
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());

            std::cin >> inputFile; // Get filename

            cout << "Please insert the name." << endl;
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());

            std::cin >> inputName; // Get name

            cout << "Please insert the age." << endl;
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());

            std::cin >> inputAge; // Get age

            data.AddRecord(inputFile, inputName, inputAge); // Add the new record to the database waiting list
        }

        // Saves the current record and new records to a new database
        if (IsKeyPressed(KEY_S))
        {
            string newDatabase;

            cout << "Please insert the name for your new database (Do not include a file-type)." << endl;
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());

            std::cin >> newDatabase; // Get filename

            data.Save(currentDatabase, newDatabase); // Save to the new database
        }

        char name[255]; // Create a char array
        memset(name, 0, 255);
        for (int i = 0; i < currentRecord->name.length(); i++) // Iterate over each char in currentRecord->name
        {
            if (currentRecord->name[i] >= 32 && currentRecord->name[i] <= 122) // If the char is a letter/standard symbol
            {
                name[i] = currentRecord->name[i]; // Put the char into the new array
            }
            else // If the checked char is not within the defined range, it is not a letter
            {
                name[i] = '\0'; // Add the null factor terminator to the end of the new char array
                break; // Exit the loop
            }
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE); // Sets the background to white

        DrawTexture(recordTexture, 300, 50, WHITE); // Draws the record image

        DrawText("NAME", 10, 50, 20, LIGHTGRAY); // Writes NAME followed by the record name
        DrawText(name, 10, 80, 20, LIGHTGRAY);

        DrawText("AGE", 10, 120, 20, LIGHTGRAY); // Writes AGE followed by the record age
        DrawText(to_string(currentRecord->age).c_str(), 10, 150, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
