/*
  Names:  Ang Li  Matthew Braun
  IDs:    1550746 1497171
  CMPUT 275, Winter 2019

  Project: audio visualizer
*/
#include <iostream>
#include <fstream>
#include <cstdlib>

// using namespace std;

int main()
{
    std::ifstream fin("output.txt");
    std::string line;
    while (getline(fin, line))
    {
        std::cout << line << std::endl;
    }
    return 0;
}
