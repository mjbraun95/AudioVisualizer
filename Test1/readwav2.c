#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <sndfile.h>
#include <math.h>
#include <complex.h>
#include <fftw3.h>
#include <assert.h>

// N = 2^12
#define N (1<<14)

#define SLICE (0.1)

static void
analyze(double *data, size_t items, int samplerate)
{
    fftw_complex *spectrum;
    fftw_complex *avg_spectrum;
    fftw_plan plan;
    int i;
    int offset;
    double *window;
    double *window_data;

    window = malloc(sizeof(double) * N);
    window_data = malloc(sizeof(double) * N);
    spectrum = (fftw_complex *) fftw_malloc(sizeof(fftw_complex) * N);
    avg_spectrum = malloc(sizeof(double) * N);

    bzero(avg_spectrum, sizeof(double) * N);

    // make Hamming window in advance
    for (i = 0; i < N; i++)
    {
        window[i] = 0.54 - 0.46 * cos(2 * M_PI * i / N);
    }

    int slice_count = items - N;
    if (slice_count == 0) slice_count = 1;
    for (offset = 0; offset + N < items; offset += N){
        for (i = 0; i < N; i++)
            window_data[i] = data[offset + i] * window[i];

        plan = fftw_plan_dft_r2c_1d(N, window_data, spectrum, FFTW_ESTIMATE);

        fftw_execute(plan);

        for (i = 0; i < N / 2 + 1; i++) {
            avg_spectrum[i] += cabs(spectrum[i]) / slice_count;
        }
        fftw_destroy_plan(plan);
    }

    for (offset = 0; offset + N < items; offset += (int)(samplerate * 0.1)){
        for (i = 0; i < N; i++)
        {
            window_data[i] = data[offset + i] * window[i];
        }

        plan = fftw_plan_dft_r2c_1d(N, window_data, spectrum, FFTW_ESTIMATE);

        fftw_execute(plan);

        printf("#freq\tabs\tdb\targ\n");
        for (i = 0; i < N / 2 + 1; i++) {
            double freq;
            double abs;
            double db;

            freq = i * (1.0 * samplerate) / N;
            abs = fabs(cabs(spectrum[i]) - avg_spectrum[i]);
            if (freq < 500) abs = 0.0; /* cut-off low-frequency noise */
            if (freq > 10000) abs = 0.0; /* cut-off high-frequency noise */
            db = 10 * log10(abs / (2 << 15));

            printf("%.2lf\t%lf\t%lf\t%lf\n",
                   freq,
                   abs,
                   db,
                   carg(spectrum[i]) / 2.0 / M_PI * 360.0);
        }
        fftw_destroy_plan(plan);
        printf("\n\n");
    }

    free(window);
    free(window_data);
    fftw_free(spectrum);
}

static void
process_wav_file(char *path)
{
    sf_count_t count;

    SF_INFO sfinfo;
    SNDFILE *sndfile;
    double *data;

    bzero(&sfinfo, sizeof(SF_INFO));
    sndfile = sf_open(path, SFM_READ, &sfinfo);
    if (sndfile == NULL)
    {
        perror("failed to open file");
        exit(EXIT_FAILURE);
    }
    if (sfinfo.channels != 1)
    {
        perror("only monoral wav file is accepted.");
        exit(EXIT_FAILURE);
    }

    data = malloc(sizeof(double) * sfinfo.frames);
    bzero(data, sizeof(double) * sfinfo.frames);
    count = sf_readf_double(sndfile, data, sfinfo.frames);
    assert(count == sfinfo.frames);

    analyze(data, sfinfo.frames, sfinfo.samplerate);

    sf_close(sndfile);
    free(data);
}


int main(int argc, char** argv)
{
    process_wav_file(argv[1]);
    return 0;
}