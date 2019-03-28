#include <cstdlib>
#include <iostream>
#include <vector>
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
    vector<vector<double>> f_bins_collection = input_file.f_domain(&data, &size);
    for (auto iter : f_bins_collection) {

        if (iter[bins] > 100) {
            appearance.push_back(index * (N / sample_rate));
        }

        index += 1;
    }
    
    for (auto iter : appearance) {
        cout << iter << endl;
    }

    return 0;
}