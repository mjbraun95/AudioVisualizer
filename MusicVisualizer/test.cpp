#include <cstdlib>
#include <iostream>
#include <vector>
#include "audio_file.h"

int main()
{
    audio_file input_file("LastNight.mp3", 44100);
    double* data;
    unsigned long size;
    input_file.decode(&data, &size);

    vector<vector<double>> f_bins_collection = input_file.f_domain(&data, &size);
    for (auto iter : f_bins_collection[5]) {
        cout << iter << endl;
    }
    cout << f_bins_collection[5].size() << endl;
    return 0;
}