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
    input_file.f_domain(&data, &size);
    return 0;
}