/*
  Names:  Ang Li  Matthew Braun
  IDs:    1550746 1497171
  CMPUT 275, Winter 2019

  Project: audio visualizer
*/
#include <algorithm>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <string>
#include <unordered_set>
#include <unistd.h>
#include <iostream>
#include <vector>

#include "audio_file.h"

using namespace std;

int main(int argc, char *argv[]) {
    // invalid usage
    if (argc != 2) {
        cout << "usage: " << argv[0] << " input_audio" << endl;
        exit(1);
    }

    audio_file input_file(argv[1], 44100);

    // array holding the transformed data
    double* data;
    // size of the data array
    unsigned long size;

    // decode & re-sample the audio file into time domain 
    input_file.decode(&data, &size);

    // transform the data to frequency domain
    vector<vector<double>> f_bins_collection = input_file.f_domain(&data, &size);

    // analyze the energy distributed in the investigated frequency band
    // to determine the appearance of the beat
    // a snare drum sound could be found in low midrange 301hz - 750hz
    unordered_set<int> beat_lower_band = input_file.beat_detector(f_bins_collection, 301, 750);

    cout << "press S to present the results!" << endl;
    string command;
    cin >> command;
    if (command == "S") {
        if (SDL_Init(SDL_INIT_VIDEO) == 0) {
            SDL_Window* window = NULL;
            SDL_Renderer* renderer = NULL;

            // create a new window & renderer
            if (SDL_CreateWindowAndRenderer(1024, 480, 0, &window, &renderer) == 0) {
                // close window trigger
                SDL_bool done = SDL_FALSE;

                SDL_Event event;
                while (!done) {
                    // indicate the appearance of the beat
                    SDL_Rect indicator = {1004, 460, 10, 10};

                    int startTime = SDL_GetTicks();
                    for (int i = 0; i < f_bins_collection.size(); i++) {
                        // set the background
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                        SDL_RenderClear(renderer);

                        // draw the bar graph representing magnitude vs. frequency
                        SDL_SetRenderDrawColor(renderer, 0, 255, 127, SDL_ALPHA_OPAQUE);
                        // only use the first 1024 frequency bins
                        SDL_Rect frequency_bins[1024];
                        for (int j = 0; j < 1024; j++) {
                            frequency_bins[j].x = j;
                            frequency_bins[j].y = 0;
                            frequency_bins[j].w = 1;
                            frequency_bins[j].h = f_bins_collection[i][j];
                        }
                        SDL_RenderFillRects(renderer, frequency_bins, 1024);

                        int currentTime = SDL_GetTicks();

                        // present the beat
                        if (beat_lower_band.find((currentTime - startTime) / 1000) != beat_lower_band.end()) {
                            SDL_SetRenderDrawColor(renderer, 255, 69, 0, SDL_ALPHA_OPAQUE);
                            SDL_RenderFillRect(renderer, &indicator);
                        }

                        SDL_RenderPresent(renderer);

                        // delay for a time unit
                        while (SDL_GetTicks() - currentTime < input_file.get_time_unit() * 1000) {
                            SDL_PollEvent(&event);
                        }

                        // close window
                        if (event.type == SDL_QUIT) {
                            break;
                        }
                    }
                    done = SDL_TRUE;
                }
            }
            else {
                cout << "failed to initialize the window..." << endl;
            }

            if (renderer) {
                SDL_DestroyRenderer(renderer);
            }
            if (window) {
                SDL_DestroyWindow(window);
            }
        }
        SDL_Quit();
    }
  return 0;
}