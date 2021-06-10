/*
** EPITECH PROJECT, 2019
** portaudio
** File description:
** portaudio
*/

#pragma once

#include <portaudio.h>
#include <opus/opus.h>
#include <opus/opus_types.h>

#include <thread>
#include <iostream>
#include <vector>

#define AUDIO_TYPE          paInt32
#define CHANNELS            1
#define BUFFER_SIZE         960
#define SAMPLE_RATE         48000
#define MIN_LATENCY         0.1

typedef struct __attribute__((packed)) userData_s
{
    OpusDecoder *decoder;
    OpusEncoder *encoder;
    std::vector<std::vector<unsigned char>> *audio;
} userData_t;

static int playCallback(const void *, void *outputBuffer,
                        unsigned long frameCount,
                        const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags,
                        void *userData)
{
    userData_t *userStruct = (userData_t *)(userData);
    std::vector<std::vector<unsigned char>> *audioData = userStruct->audio;
    int16_t *data = (int16_t *)outputBuffer;

    if (userStruct->encoder != NULL) {
        throw std::exception();
    }
    if (audioData->empty()) {
        for (unsigned long i = 0; i < frameCount; ++i, ++data) {
            *data = 0;
        }
        return (paContinue);
    }

    std::vector<unsigned char> audio = audioData->front();
    audioData->erase(audioData->begin());
    
    int error = opus_decode(userStruct->decoder, (unsigned char *)audio.data(), audio.size() * 2, data, frameCount, 0);
    if (error < 0) {
        std::cerr << "Decode error: " << error << std::endl;
        for (unsigned long i = 0; i < frameCount; ++i, ++data ) {
            *data = 0;
        }
    }
    return (paContinue);
}

static int recordCallback(const void *inputBuffer, void *,
                          unsigned long frameCount,
                          const PaStreamCallbackTimeInfo *,
                          PaStreamCallbackFlags,
                          void *userData)
{
    userData_t *userStruct = (userData_t *)userData;
    int16_t *data = (int16_t *)inputBuffer;
    std::vector<unsigned char> audio;
    audio.resize (frameCount * 2);
    
    if (userStruct->decoder != NULL) {
        throw std::exception();
    }
    int encoded = opus_encode(userStruct->encoder, data, frameCount, (unsigned char *)audio.data(), audio.size() * 2);

    if (encoded <= 0) {
        std::cerr << "Encode error: " << encoded << std::endl;
        return (paContinue);
    }
    
    std::vector<std::vector<unsigned char>> *audioData = userStruct->audio;
    audio.resize(encoded);
    audioData->push_back(audio);
    return (paContinue);
}

class PortAudio
{
private:
    PaStream *_play = nullptr;
    PaStream *_record = nullptr;
    std::vector<std::vector<unsigned char>> _audioData;
    
    bool createRecordStream(OpusEncoder *);
    bool createPlayStream(OpusDecoder *);

public:
    PortAudio(void);
    ~PortAudio(void);
    bool createStreams(OpusEncoder *, OpusDecoder *);
    bool startStreams(void);
    void stopStreams(void);
};
