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