#include <fstream>
#include <string>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void FFT()
{
    vector<double> bins;
    double N=1024, Fs=44100, freq=0;
    int i=0;
    while (freq < 20000)
    {
        freq = i*Fs/N;
        bins.push_back(freq);
        i++;
    }
    cout << "Total vector length: " << i << endl;

}

int main()
{
    ifstream fin("output.txt");
    string line, Lline, Rline;
    int at=0;
    char lastC;
    bool posNum=false, negNum=false, rightSide=true;
    int debug;
    vector<pair<double, double>> magPairArray;
    while (getline(fin, line)) 
    {
    // split the string around the commas, there will always be 4 substrings
        // string p[4];
        
        
        
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
        pair<double, double> thisPair;
        double Ldoub = stod(Lline);
        double Rdoub = stod(Rline);
        thisPair.first = Ldoub;
        thisPair.second = Rdoub;
        magPairArray.push_back(thisPair);

        at++;
        cout << "At line " << at << ": " << thisPair.first << ", " << thisPair.second << ";" << endl;
        // cout << "At line " << at << ":" << Ldoub << "," << Rdoub << "!" << endl;
        Lline.clear();
        Rline.clear();
        // cin >> debug;
        FFT();
    }
    return 0;
}