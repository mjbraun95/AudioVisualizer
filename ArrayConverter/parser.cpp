#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    ifstream fin("output.txt");
    string line, Lline, Rline;
    int at=0;
    char lastC;
    bool posNum=false, negNum=false, rightSide=true;
    while (getline(fin, line)) 
    {
    // split the string around the commas, there will always be 4 substrings
        // string p[4];
        
        at++;
        
        for (auto c : line) 
        {
            
            if (c == ' ')// and lastC == ' ') 
            {
                
                posNum=false;
                negNum=false;
                // if (lastC == ' ')
                // cout << "spez" << endl;
            }
            else if ((c == '0' or c == '1') and lastC == ' ')
            {
                posNum=true;
                rightSide = !rightSide;
            }
            else if (c == '-')
            {
                negNum=true;
                rightSide = !rightSide;
            }
            if (negNum == true or posNum == true)
            {
                if (!rightSide)
                {
                    Lline += c;
                }
                else
                {
                    Rline += c;
                }
            }
            // else if (posNum=true)
            // {
            //     if (!rightSide)
            //     {
            //         Lline += c;
            //     }
            //     else
            //     {
            //         Rline += c;
            //     }
            // }
            // else if (c == '\n')
            // {
            //     break;
            // }
            lastC = c;
            // Don't want to index out of bounds!
            // assert(at < 4);
        }
        cout << "At line " << at << ": " << Lline << " AND " << Rline << endl;
        Lline.clear();
        Rline.clear();
        
    }
    return 0;
}