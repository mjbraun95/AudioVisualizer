#include <iostream>
#include <complex>
#include <vector>

#include "FFT.h"

using namespace std;

int main()
{
    vector<complex<double>> t_domain = {1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0};
    FFT(t_domain);
    for (auto iter : t_domain) {
        cout << iter << endl;
    }
    return 0;
}