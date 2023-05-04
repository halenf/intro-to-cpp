// std.vector tutorial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;
using Iterator = vector<int>::iterator;

void PrintVector(string message, vector<int> v);
bool DescFunction(int i, int j);
void SetZero(int i);

int main()
{
    // Setup
    vector<int> v;
    Iterator it;
    
    // Random values
    for (int i = 0; i < 20; i++)
    {
        v.push_back(rand() % 101);
    }
    PrintVector("Initial values:", v);
    
    // Using std::sort to sort ascending
    sort(v.begin(), v.end());
    PrintVector("Sorted ascending:", v);

    // Setting every second value to 0
    for (int i = 0; i < v.size(); i++)
    {
        if ((i + 1) % 2 == 0)
        {
            v[i] = 0;
        }
    }
    PrintVector("Every second value set to 0:", v);

    // Moving on to part 2
    cout << "Press Enter to continue...";
    cin.get();
    cin.clear();
    cin.ignore(cin.rdbuf()->in_avail());
    cout << endl;

    // Setup 2
    v.clear();

    // Random values
    for (int i = 0; i < 20; i++)
    {
        v.push_back(rand() % 101);
    }
    PrintVector("Inital values:", v);

    // Sort descending
    sort(v.begin(), v.end(), DescFunction);
    PrintVector("Sorted descending:", v);

    // Set every second value to 0 with std::for_each
    int index = 0;
    for_each(v.begin(), v.end(), [&index](int& i)
        {
            if ((index + 1) % 2 == 0)
            {
                i = 0;
            } 
            index++; 
        }
    );
    PrintVector("Every second value set to 0 using std::for_each:", v);

    cout << endl << "Press Enter to quit...";
    cin.get();

    return 0;
}

void PrintVector(string message, vector<int> v)
{
    cout << message << " ";
    for (auto x : v)
    {
        cout << x << ", ";
    }
    cout << endl;
}

bool DescFunction(int i, int j)
{
    return (i > j);
}

