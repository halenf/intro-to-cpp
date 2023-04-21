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
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "NPC Data-Checker");

    DataFile data;
    int currentRecordIdx = 0;
    data.GetDatabaseInfo("npc_data.dat");

    DataFile::Record* currentRecord = data.Load("npc_data.dat", currentRecordIdx);
    Texture2D recordTexture = LoadTextureFromImage(currentRecord->image);

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
                currentRecord = data.Load("npc_data.dat", currentRecordIdx);
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
                currentRecord = data.Load("npc_data.dat", currentRecordIdx);
                recordTexture = LoadTextureFromImage(currentRecord->image);
            }
        }

        char name[255]; // Create a char array
        for (int i = 0; i < currentRecord->name.length() - 1; i++) // Iterate over each char in currentRecord->name
        {
            if (currentRecord->name[i] >= 65 && currentRecord->name[i] <= 122) // If the char is a letter/standard symbol
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

        ClearBackground(RAYWHITE);

        DrawTexture(recordTexture, 300, 50, WHITE);

        DrawText("NAME", 10, 50, 20, LIGHTGRAY);
        DrawText(name, 10, 80, 20, LIGHTGRAY);

        DrawText("AGE", 10, 120, 20, LIGHTGRAY);
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