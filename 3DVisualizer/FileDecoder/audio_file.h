/*
  Names:  Ang Li  Matthew Braun
  IDs:    1550746 1497171
  CMPUT 275, Winter 2019

  Project: audio visualizer
*/
#ifndef _AUDIO_FILE_H_
#define _AUDIO_FILE_H_

#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <unordered_set>
#include <utility>

#include "FFT.h"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswresample/swresample.h>
    #include <libavutil/opt.h>
}

using namespace std;

/*  
    audio file decoder & transfer & analyzer
    decode part build upon the Libavcodec tutorials retrieved from
        https://rodic.fr/blog/libavcodec-tutorial-decode-audio-file/
        dranger.com/ffmpeg/tutorial01.html

    decode & re-sample the audio file into time domain (amplitude vs. time)
    with the specified sample rate & mono channel
    then transform it into frequency domain & process the data
    
    required libraries:
        $sudo apt-get install libavcodec-dev libavformat-dev libavutil-dev libswresample-dev
*/
class audio_file {
public:
    // file_name: the file to be decoded
    // sample_rate: re-sampled rate
    // (i.e. # of samples of audio carried per second, measured in Hz)
    audio_file(const char* file_name, const int sample_rate) {
        this->file_name = file_name;
        this->sample_rate = sample_rate;
    }

    // decode & re-sample the audio file into time domain 
    //
    // data: array holding the transformed data
    // size: size of the data array
    void decode(double** data, unsigned long* size);

    // transform the data into frequency domain
    // return the two dimensional vector containing the transformed frequency bins
    //
    // to access the frequency bins at time t: f_bins_collection[t / time unit]
    // to access i'th frequency bin (index i) at time t: f_bins_collection[t / time unit][i]
    vector<vector<double>> f_domain(double** data, unsigned long* size);

    /* 
        beat detect algorithm retrieved from:
            https://www.parallelcube.com/2018/03/30/beat-detection-algorithm/

        analyze the energy distributed in the investigated frequency band
        to determine the appearance of the beat

        f_bins_collection: the FFT frequency bins collection
        band_min: lower bound of the investigated frequency band
        band_max: upper bound of the investigated frequency band
        return the time when the beat appears (in 10 ^ -2 s)
    */
    unordered_set<int> beat_detector(vector<vector<double>> &f_bins_collection, int band_min, int band_max);

    // extract the behavior of certain frequencies
    //
    // f_bins_collection: the FFT frequency bins collection
    // frequencies: the frequencies to be investigated
    // return the frequency bins collection representing required frequencies
    vector<vector<double>> filter(vector<vector<double>> &f_bins_collection, vector<int> &frequencies);

    // return the time unit
    // which is determined by the # of frequency bins collected in a time period
    double get_time_unit() {
        return time_unit;
    }

private:
    const char* file_name;
    int sample_rate;

    // the implemented FFT algorithm could only handle the vector
    // with the size of 2 ^ m for m > 0
    int FFT_size = 2048;

    // determined by the # of frequency bins collected in a time period
    double time_unit;
    // the audio duration (in time_unit)
    int duration;
};

// ***** PUBLIC METHODS FIRST *****

void audio_file::decode(double** data, unsigned long* size) {
    cout << "Decoding audio..." << endl;
    // register all available file formats & codecs
    av_register_all();

    // read the audio file header & store the info about its file format
    AVFormatContext* formatCtx = avformat_alloc_context();
    if (avformat_open_input(&formatCtx, file_name, NULL, NULL) != 0) {
        cout << "Failed to open the file" << endl;
        exit(1);
    }

    // retrieve stream info
    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        cout << "Failed to retrieve stream info" << endl;
        exit(1);
    }

    // walk through the AVFormatContext structure until find an audio stream
    unsigned int aStream = 0;
    for (; aStream < formatCtx->nb_streams; aStream++) {
        if (formatCtx->streams[aStream]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            break;
        }   
    }
    if (aStream == formatCtx->nb_streams) {
        cout << "Failed to find an audio stream" << endl;
        exit(1);
    }

    // get the codec context for the audio stream,
    // which contains the info about the codec that the stream is using
    AVCodecContext *codecCtx = formatCtx->streams[aStream]->codec;

    // find the decoder for the audio stream
    AVCodec *codec = avcodec_find_decoder(codecCtx->codec_id);
    if (!codec) {
        cout << "Failed to find the corresponding codec" << endl;
        exit(1);
    }

    // open codec
    if(avcodec_open2(codecCtx, codec, NULL) < 0) {
        cout << "Failed to open codec" << endl;
        exit(1);
    }

    // prepare re-sampler
    SwrContext *swr = swr_alloc_set_opts(NULL, // allocate a new context
            AV_CH_LAYOUT_MONO, // out_ch_layout
            AV_SAMPLE_FMT_DBL, // out_sample_fmt
            sample_rate, // out_sample_rate
            codecCtx->channel_layout, // in_ch_layout
            codecCtx->sample_fmt, // in_sample_fmt
            codecCtx->sample_rate, // in_sample_rate
            0, // log_offset
            NULL); //log_ctx

    // initialize the re-sampler
    swr_init(swr);

    if (!swr) {
        cout << "Failed to initialize the re-sampler" << endl;
        exit(1);
    }

    // packet contains bits of data that are decoded into raw frames
    AVPacket packet;
    av_init_packet(&packet);

    // the decoded data would be written into frame
    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        cout << "Failed to allocate frame" << endl;
        exit(1);
    }

    // store the decoded & re-sampled audio data in time domain
    *data = NULL;
    *size = 0;

    // iterate through frames
    while (av_read_frame(formatCtx, &packet) >= 0) {
        // skip non-audio packets
        if (packet.stream_index != aStream) {
            continue;
        }

        // decode one frame
        int gotFrame = 0;
        if (avcodec_decode_audio4(codecCtx, frame, &gotFrame, &packet) < 0) {
            cout << "Failed to decode the frame" << endl;
            exit(1);
        }

        if (!gotFrame) {
            continue;
        }

        // re-sample frames
        double* buffer;
        av_samples_alloc((uint8_t**) &buffer, // audio_data
             NULL, // line size
             1, // nb_channels
             frame->nb_samples, // nb_samples
             AV_SAMPLE_FMT_DBL, // sample_fmt
             0); // align

        int frame_count = swr_convert(swr, // struct SwrContext *s
            (uint8_t**) &buffer, // uint8_t *out_arg[SWR_CH_MAX]
            frame->nb_samples, // int out_count
            (const uint8_t**) frame->data, // uint8_t *in_arg [SWR_CH_MAX]
            frame->nb_samples); // int in_count

        if (frame_count < 0) {
            cout << "Failed to re-sample the data" << endl;
            exit(1);
        }

        // append re-sampled frames to data
        *data = (double*) realloc(*data, (*size + frame->nb_samples) * sizeof(double));
        memcpy(*data + *size, buffer, frame_count * sizeof(double));
        *size += frame_count;

        // free spaces
        av_free(buffer);
        av_free_packet(&packet);
    }

    cout << "The size of the decoded & re-sampled audio data: " << *size << endl;

    av_frame_free(&frame);
    swr_free(&swr);

    // close the codecs
    avcodec_close(codecCtx);

    // close the audio file
    avformat_free_context(formatCtx);

    cout << "Done!" << endl;
}

vector<vector<double>> audio_file::f_domain(double** data, unsigned long* size) {
    cout << "Transforming the data into the frequency domain..." << endl;
    // e.g. if FFT_size = 2048 & this->sample_rate = 44100, then the time_unit would be 0.046 s
    time_unit = (double) FFT_size / (double) sample_rate;
    duration = *size / FFT_size;

    // store vectors contain the frequency bins for each time unit
    vector<vector<double>> f_bins_collection(duration, vector<double> (FFT_size / 2, 0));
    for (int i = 0; i < duration; i++) {
        // feed the data storing the audio info for one time unit
        // into the vector of complex number
        vector<complex> sample;
        for (int j = 0; j < FFT_size; j++) {
            sample.push_back(complex((*data)[i * FFT_size + j]));
        }

        // prevent spectral leakage
        window(sample);
        // Fast Fourier transform
        FFT(sample);

        // store the magnitude of each complex number
        int k = 0;
        for (auto iter : sample) {
            f_bins_collection[i][k] = iter.abs();

            k += 1;
            // the useful index range for frequency bins is from 1 to N/2
            if (k > FFT_size / 2 - 1) {
                break;
            }
        }
    }
    
    cout << "Done!" << endl;
    return f_bins_collection;
}

unordered_set<int> audio_file::beat_detector(vector<vector<double>> &f_bins_collection,
    int band_min, int band_max) {
    cout << "Detecting beats..." << endl;

    // store the time when the beat appears
    unordered_set<int> beat_appearance;
    // energy distributed in the investigated frequency band per time unit
    vector<double> energy;

    // band_max * FFT_size / sample_rate: index of the maximum frequency in the frequency bins
    // band_min * FFT_size / sample_rate: index of the minimum frequency in the frequency bins
    int index_num = (band_max * FFT_size / sample_rate) - (band_min * FFT_size / sample_rate);

    // total energy distributed in the investigated frequency band
    double totalE = 0;
    for (int i = 0; i < f_bins_collection.size(); i++) {
        double f_magnitude_sum = 0;
        // get the energy distributed in certain frequency
        for (int j = band_min * FFT_size / sample_rate; j < band_max * FFT_size / sample_rate; j++) {
            f_magnitude_sum += f_bins_collection[i][j];
        }

        energy.push_back(f_magnitude_sum / index_num);
        totalE += (f_magnitude_sum / index_num);
    }

    // avenge energy distributed in the investigated frequency band per time unit
    totalE = totalE / f_bins_collection.size();

    // compute the variance & threshold
    double variance = 0;
    for (int i = 0; i < f_bins_collection.size(); i++) {
        variance += pow(energy[i] - totalE, 2);
    }
    variance = variance / f_bins_collection.size();

    // formula retrieved from:
    //      http://mziccard.me/2015/05/28/beats-detection-algorithms-1/
    double threshold = -0.0000015 * variance / f_bins_collection.size() + 1.5142857;

    // find the qualified beats & get the time they appear
    for (int i = 0; i < energy.size(); i++) {
        if (energy[i] > threshold * totalE) {
            beat_appearance.insert(i * time_unit * 10); // round to 0.1 s
        }
    }

    cout << "Done!" << endl;
    return beat_appearance;
}

vector<vector<double>> audio_file::filter(vector<vector<double>> &f_bins_collection, vector<int> &frequencies) {
    cout << "Extracting the behavior of required frequencies..." << endl;
    // store the frequency bins collection representing required frequencies
    vector<vector<double>> f_behavior(f_bins_collection.size(), vector<double> (frequencies.size(), 0));
    for (int i = 0; i < f_bins_collection.size(); i++) {
        for (int j = 0; j < frequencies.size(); j++) {
            // the i'th bin in the FFT results representing the behavior of the frequency Fs where
            // i * sample_rate / FFT_size = Fs
            f_behavior[i][j] = f_bins_collection[i][frequencies[j] * FFT_size / sample_rate];
        }
    }

    cout << "Done!" << endl;
    return f_behavior;
}

// ***** NOW THE PRIVATE METHODS *****

#endif
