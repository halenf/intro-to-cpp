

#include "Application.h"

#include <iostream>
#include "Dictionary.h"
using namespace Aaron;



int main(int argc, char* argv[])
{

    {

        Dictionary dictionary("dictionary.txt");

        std::cout << std::boolalpha;
        std::cout << dictionary.Contains("hello") << std::endl;
        std::cout << dictionary.Contains("pilgrimaged") << std::endl;
        std::cout << dictionary.Contains("ejector") << std::endl;
        std::cout << dictionary.Contains("thisworddoesntexist") << std::endl;
        std::cout << dictionary.Contains("z") << std::endl;
    }

    // return 0;

    {
        Application app;
        app.Run();
    }

    return 0;
}