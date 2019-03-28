#ifndef _FFT_H_
#define _FFT_H_

#include <complex>
#include <cmath>
#include <vector>
#include <utility> // std::pair

using namespace std;

/*
     applying the window function to the discrete time signals
     to prevent spectral leakage
*/
void window(vector<complex<double>> &sample){
    int N = sample.size();

    for (int i = 0; i < N; i++) {
        double w_n = 0.5 * (1 - cos(2 * M_PI * i / (N - 1)));
        sample[i] *= w_n;
    }
}

/*
    interpretation of Fast Fourier transform using Cooley-Tukey Algorithms
    build upon the pseudo code retrieved form:
        http://people.scs.carleton.ca/~maheshwa/courses/5703COMP/16Fall/FFT_Report.pdf
        (notice that the exponential term should be e ^ (-2 * PI * i/N) rather than e ^ (2 * PI * i/N))

    sample: An array of complex values [which has a size of 2 ^ m for m > 0]

    convert the input from time domain to frequency domain (frequency bins)
*/
void FFT(vector<complex<double>> &sample) {
    const int N = sample.size();

    // the FFT of an array with a single value is the array itself
    if (N == 1) {
        return;
    }
    else {
        // repeat diving up the DFT into even index sum & odd index sum
        // until there is only one sample left
        vector<complex<double>> even(N/2, 0);
        vector<complex<double>> odd(N/2, 0);
        for (int i = 0; i < N/2; i++) {
            even[i] = sample[2 * i];
            odd[i] = sample[2 * i + 1];
        }

        FFT(even);
        FFT(odd);

        complex<double> W_N = exp(-2 * M_PI * complex<double> (0, 1)/complex<double> (N, 0));
        // exponential term for k = 0
        complex<double> W  = 1;

        for (int k = 0; k < N/2; k++) {
            // combine the value until arrive at the expected frequency bins
            sample[k] = even[k] + W * odd[k];
            // if k > N/2, the experiential term is a repeat of k - N/2
            // sum becomes a repeat for all k > N/2
            sample[k + N/2] = even[k] - W * odd[k];

            // update the exponential term
            W *= W_N;
        }
    }
}

#endif
