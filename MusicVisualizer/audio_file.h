#ifndef _AUDIO_FILE_H_
#define _AUDIO_FILE_H_

#include <cmath>
#include <complex>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "FFT.h"

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswresample/swresample.h>
    #include <libavutil/opt.h>
}

using namespace std;

/*  
    audio file decoder & transfer
    decode part build upon the Libavcodec tutorial from
        https://rodic.fr/blog/libavcodec-tutorial-decode-audio-file/
        dranger.com/ffmpeg/tutorial01.html

    decode & re-sample the audio file into time domain (amplitude vs. time)
    with the specified sample rate & mono channel
    then transform it into frequency domain
    
    required libraries:
        $apt-get install libavcodec-dev libavformat-dev libavutil-dev libswresample-dev
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
    // data: array holding the transformed data
    // size: size of the data
    void decode(double** data, unsigned long* size);

    // transform the data to frequency domain
    // return the two dimensional vector
    // containing the transformed frequency bins at time t (in second)
    // e.g. the frequency bins at time 0:03 could be retrieved
    // by calling f_bins_collection[3]
    vector<vector<double>> f_domain(double** data, unsigned long* size);

private:
    const char* file_name;
    int sample_rate;
};

// ***** PUBLIC METHODS FIRST *****

void audio_file::decode(double** data, unsigned long* size) {
    // register all available file formats & codecs
    av_register_all();

    // read the auto file header & store info about its file format
    AVFormatContext* formatCtx = avformat_alloc_context();
    if (avformat_open_input(&formatCtx, this->file_name, NULL, NULL) != 0) {
        cout << "Failed to open the file" << endl;
        exit(1);
    }

    // retrieve stream info
    if (avformat_find_stream_info(formatCtx, NULL) < 0) {
        cout << "Failed to retrieve stream info" << endl;
        exit(1);
    }

    // walk through the AVFormatContext structure until find an audio stream
    size_t aStream = 0;
    for (aStream; aStream < formatCtx->nb_streams; aStream++) {
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
            this->sample_rate, // out_sample_rate
            codecCtx->channel_layout, // in_ch_layout
            codecCtx->sample_fmt, // in_sample_fmt
            codecCtx->sample_rate, // in_sample_rate
            0, // log_offset
            NULL); //log_ctx

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
             // 1, // nb_channels
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

    // for (int i = 0; i  < *size; i++) {
    //     cout << (*data)[i] << endl;
    // }

    cout << "The size of the decoded & re-sampled audio data: " << *size << endl;

    av_frame_free(&frame);
    swr_free(&swr);

    // close the codecs
    avcodec_close(codecCtx);

    // close the audio file
    avformat_free_context(formatCtx);
}

vector<vector<double>> audio_file::f_domain(double** data, unsigned long* size) {
    // the audio duration (in second)
    int duration = (int) *size / this->sample_rate;
    // the interpreted FFT algorithm could only handle the vector
    // with the size of 2 ^ m for m > 0
    int FFT_size = pow(2.0, (int) (log(this->sample_rate) / log(2)));

    // store vectors contain the frequency bins for each second
    vector<vector<double>> f_bins_collection(duration, vector<double> (FFT_size, 0));
    for (int i = 0; i < duration; i++) {
        // feed the data storing the audio info for one second
        // into the vector of complex number
        vector<complex<double>> sample;
        for (int j = 0; j < FFT_size; j++) {
            // since FFT_size < this->sample_rate
            // only extract the middle part of the audio info
            sample.push_back((*data)[i * this->sample_rate
                + (this->sample_rate - FFT_size)/2 + j]);
        }

        FFT(sample);

        // store the magnitude of each complex number
        int k = 0;
        for (auto iter : sample) {
            f_bins_collection[i][k] = abs(iter);
            k += 1;
        }
    }
    
    return f_bins_collection;
}

// ***** NOW THE PRIVATE METHODS *****

#endif
