#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include "audio_file.h"

int main()
{
    audio_file input_file("440.mp3", 44100);
    double* data;
    unsigned long size;
    input_file.decode(&data, &size);

    double N = 2048;
    double sample_rate = 44100;

    vector<double> appearance;
    int bins = 100 / (sample_rate / N);

    double index = 0;
    // to access the frequency bins at time t
    // use f_bins_collection[t]
    // to access a certain frequency bin (index i) at time t
    // use f_bins_collection[t][i]

    vector<vector<double>> f_bins_collection = input_file.f_domain(&data, &size);
    cout << f_bins_collection.size() << endl;
    cout << f_bins_collection[0].size() << endl;
    // for (int i=0; i<)

    //f_bins size: 433
    //f_bins[0] size: 1024
    ofstream output;
    output.open("output.txt");
    for (auto iter : f_bins_collection[430]) {
        cout << "iter[200]: " << iter << "; freq: " << (index*(sample_rate/N)) << "; index: " << index << endl;
        output << (index*(sample_rate/N)) << ":" << iter << "\n";
    //     if (iter[bins] > 100) {
    //         appearance.push_back(index * (N / sample_rate));
    //     }

        index += 1;
    }
    
    // for (auto iter : appearance) {
    //     cout << iter << endl;
    // }

    return 0;
}