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
    double freq = 0;
    vector<vector<double>> f_bins_collection = input_file.f_domain(&data, &size);
    for (auto iter : f_bins_collection[6]) {
        if (freq < 1000)
        cout << iter << " "<< freq << endl;
        freq=freq+1.345825195;
    }
    cout << f_bins_collection[5].size() << endl;
    cout << f_bins_collection.size() << endl;
    cout << input_file.get_time_unit() << endl;
    return 0;
}