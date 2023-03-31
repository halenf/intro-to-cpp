// PointerExercise.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Monster.h"
#include <iostream>
#include <conio.h>

void AttackMonster(Monster * defender, Monster * attacker);
void DisplayMonsterHealth(Monster * m);

int main()
{
    //Create monsters
    Monster dragon{ "Dragon", 200, 20 };
    Monster vampire{ "Vampire", 150, 10 };
    Monster zombie{ "Zombie", 50, 8 };
    Monster impossibleSlime{ "Impossible Slime", 5000, 500 };

    //create players
    Monster warrior{ "Warrior", 110, 22 };
    Monster rogue{ "Rogue", 80, 28 };
    Monster mage{ "Mage", 100, 25 };
    Monster god{ "God" , 10000, 800 };

    //pick a random enemy
    Monster * currentMonster;
    srand(time(0)); //this generate a random seed
    int r = rand() % 3; //this generates a random number between 0-3 exclusive
    switch (r)
    {
        case 0: currentMonster = &dragon; break;
        case 1: currentMonster = &vampire; break;
        case 2: currentMonster = &zombie; break;

        //without a default, C++ assumes the possibility that current is never assigned
        // and therefore returning an error later when we try to access it
        default: currentMonster = &impossibleSlime; break; 
    }

    // pick a random player class
    Monster* currentPlayer;
    srand(time(0)); //this generate a random seed
    r = rand() % 3; //this generates a random number between 0-3 exclusive
    switch (r)
    {
        case 0: currentPlayer = &warrior; break;
        case 1: currentPlayer = &rogue; break;
        case 2: currentPlayer = &mage; break;

        //without a default, C++ assumes the possibility that current is never assigned
        // and therefore returning an error later when we try to access it
        default: currentPlayer = &god; break;
    }

    //fight until someone is defeated
    while (currentMonster->health > 0 && currentPlayer->health > 0)
    {
        //player attacks current monster
        AttackMonster(currentMonster, currentPlayer);
        DisplayMonsterHealth(currentMonster);

        //wait for input then clear screen
        system("pause");
        system("cls");

        if (currentMonster->health > 0)
        {
            //current monster attacks player
            AttackMonster(currentPlayer, currentMonster);
            DisplayMonsterHealth(currentPlayer);

            //wait for input then clear screen
            system("pause");
            system("cls");
        }
    }
}

void AttackMonster(Monster * defender, Monster * attacker)
{
    defender->health -= attacker->damage;

    std::cout << attacker->name << " attacks the " << defender->name << " for " 
        << attacker->damage << " damage!" << std::endl;
}

void DisplayMonsterHealth(Monster * m)
{
    std::cout << m->name << " has " << m->health << " health remaining..." << std::endl;
    
    if (m->health < 0)
    {
        std::cout << "The " << m->name << " was defeated!\n";
    }
}
