/*
  Names:  Ang Li  Matthew Braun
  IDs:    1550746 1497171
  CMPUT 275, Winter 2019

  Project: audio visualizer
*/
#ifndef _FFT_H_
#define _FFT_H_

#include <cmath>
#include <vector>
#include <utility> // std::pair

using namespace std;

/*
    contains the pair storing the real & imaginary parts of the complex number
    and implements each operation as a separate member method
*/
class complex {
public:
    complex (double real, double complex = 0.0) {
        this->complexPair.first = real;
        this->complexPair.second = complex;
    };

    // addition:
    // Simply add the real & imaginary parts separately
    complex operator+(const complex& num) {
        return complex(complexPair.first + num.complexPair.first,
            complexPair.second + num.complexPair.second);
    }

    // subtraction:
    // Simply subtract the real & imaginary parts separately
    complex operator-(const complex& num) {
        return complex(complexPair.first - num.complexPair.first,
            complexPair.second - num.complexPair.second);
    }

    // multiplication;
    // Multiply through the brackets (i.e., FOIL) and collect like terms
    complex operator*(const complex& num) {
        // (a + bi) * (c + di) = (ac - bd) + (ad + bc)i
        return complex(complexPair.first * num.complexPair.first 
            - complexPair.second * num.complexPair.second,
            complexPair.first * num.complexPair.second 
            + complexPair.second * num.complexPair.first);
    }

    // exponentiation:
    // compute base-e exponential of z
    // i.e. e (Euler's number) raised to the z power
    complex cExp() {
        // e ^ z for z = x + iy equals (e ^ x) * (cos(y) + isin(y))
        double exponential_term = exp(complexPair.first);
        return complex(exponential_term * cos(complexPair.second),
            exponential_term * sin(complexPair.second));
    }

    // return the magnitude of the complex number
    double abs() {
        return(sqrt(pow(complexPair.first, 2) + pow(complexPair.second, 2)));
    }

    // return the complex number as a pair of its real & imaginary parts
    pair <double, double> getComplexPair() {
        return complexPair;
    }

private:
    // store the real & imaginary parts of the complex number
    pair <double, double> complexPair;
};

/*
    interpretation of the Fast Fourier transform using Cooley-Tukey Algorithms
    build upon the pseudo code retrieved form:
        https://courses.cs.washington.edu/courses/cse373/16au/slides/23-Divide-and-Conquer-the-FFT-6up.pdf

    sample: An array of complex values [which has a size of 2 ^ m for m > 0]

    convert the input from time domain to frequency domain (frequency bins)
*/
void FFT(vector<complex> &sample) {
    const int N = sample.size();

    // the FFT of an array with a single value is the array itself
    if (N == 1) {
        return;
    }
    else {
        // repeat diving up the DFT into even index sum & odd index sum
        // until there is only one sample left
        vector<complex> even(N/2, complex(0));
        vector<complex> odd(N/2, complex(0));
        for (int i = 0; i < N/2; i++) {
            even[i] = sample[2 * i];
            odd[i] = sample[2 * i + 1];
        }

        FFT(even);
        FFT(odd);

        complex W = (complex(-2 * M_PI / (double) N) * complex(0, 1)).cExp();
        // exponential term for k = 0
        complex W_k  = complex(1);

        for (int k = 0; k < N/2; k++) {
            // combine the value until arrive at the expected frequency bins
            sample[k] = even[k] + (W_k * odd[k]);
            // if k > N/2, the experiential term is a repeat of k - N/2
            // sum becomes a repeat for all k > N/2
            sample[k + N/2] = even[k] - (W_k * odd[k]);

            // update the exponential term
            W_k = W_k * W;
        }
    }
}

/*
     apply the window function to the discrete time signals
     preventing spectral leakage
*/
void window(vector<complex> &sample){
    int N = sample.size();

    for (int i = 0; i < N; i++) {
        // exponential term
        double W_n = 0.5 * (1 - cos(2 * M_PI * i / (N - 1)));
        sample[i] =  sample[i] * complex(W_n);
    }
}

#endif
