/*
** EPITECH PROJECT, 2019
** portaudio
** File description:
** portaudio
*/

#include "PortAudio.hpp"

PortAudio::PortAudio(void)
{
    int error = Pa_Initialize();

    if (error != paNoError) {
        std::cerr << Pa_GetErrorText(error) << std::endl;
        throw std::exception();
    }
}

PortAudio::~PortAudio(void)
{
    if (this->_play) {
        Pa_CloseStream(this->_play);
        this->_play = nullptr;
    }
    if (this->_record) {
        Pa_CloseStream(this->_record);
        this->_record = nullptr;
    }
}

bool PortAudio::createPlayStream(OpusDecoder *decoder)
{
    PaDeviceIndex index = Pa_GetDefaultOutputDevice();
    const PaDeviceInfo *info = Pa_GetDeviceInfo(index);

    if (!info) {
        std::cerr << "Could not find the playback device." << std::endl;
        return false;
    }
    if (info->maxOutputChannels == 0) {
        std::cerr << "The playback device is currently busy";
        return false;
    }

    PaStreamParameters params;
    {
        params.device = Pa_HostApiDeviceIndexToDeviceIndex(info->hostApi, index);
        params.channelCount = CHANNELS;
        params.sampleFormat = AUDIO_TYPE;
        params.suggestedLatency = MIN_LATENCY;
        params.hostApiSpecificStreamInfo = nullptr;
    }

    userData_t *userData = new userData_t;
    userData->udp = NULL;
    userData->decoder = decoder;
    userData->encoder = NULL;
    userData->audio = &(this->_audioData);
    PaError errorOpenStream = Pa_OpenStream(&this->_play,
                                            nullptr,
                                            &params,
                                            SAMPLE_RATE,
                                            BUFFER_SIZE,
                                            paClipOff,
                                            playCallback,
                                            userData
        );
    
    if (errorOpenStream != paNoError) {
        std::cerr << "Could not create streaming stream: " << Pa_GetErrorText(errorOpenStream) << std::endl;
        return false;
    }
    return true;
}

bool PortAudio::createRecordStream(OpusEncoder *encoder, UdpClient *p_udp)
{
    PaDeviceIndex index = Pa_GetDefaultInputDevice();
    const PaDeviceInfo *info = Pa_GetDeviceInfo(index);

    if (!info) {
        std::cerr << "Could not find recorder" << std::endl;
        return false;
    }
    if (info->maxInputChannels == 0) {
        std::cerr << "The recording device is currently busy" << std::endl;
        return false;
    }

    PaStreamParameters params;
    {
        params.device = Pa_HostApiDeviceIndexToDeviceIndex(info->hostApi, index);
        params.channelCount = CHANNELS;
        params.sampleFormat = AUDIO_TYPE;
        params.suggestedLatency = MIN_LATENCY;
        params.hostApiSpecificStreamInfo = nullptr;
    }

    userData_t *userData = new userData_t;
    userData->udp = p_udp;
    userData->decoder = NULL;
    userData->encoder = encoder;
    userData->audio = &(this->_audioData);
    PaError errorOpenStream = Pa_OpenStream(&this->_record,
                                            &params,
                                            nullptr,
                                            SAMPLE_RATE,
                                            BUFFER_SIZE,
                                            paClipOff,
                                            recordCallback,
                                            userData
        );

    if (errorOpenStream != paNoError) {
        std::cerr << "Could not create stream record: " << Pa_GetErrorText(errorOpenStream) << std::endl;
        return false;
    }
    return true;
}

bool PortAudio::createStreams(OpusEncoder *encoder, OpusDecoder *decoder, UdpClient *p_udp)
{
    if (!this->createPlayStream(decoder)) {
        return false;
    }
    if (!this->createRecordStream(encoder, p_udp)) {
        return false;
    }
    return true;
}

bool PortAudio::startStreams(void)
{
    int error = Pa_StartStream(this->_play);

    if (error != paNoError) {
        std::cerr << "Unable to start streaming playback: " << Pa_GetErrorText(error) << std::endl;
        return false;
    }
    error = Pa_StartStream(this->_record);
    if (error != paNoError) {
        std::cerr << "Could not start stream record: " << Pa_GetErrorText(error) << std::endl;
        return false;
    }
    return true;
}

void PortAudio::stopStreams(void)
{
    if (this->_play) {
        Pa_StopStream(this->_play);
    }
    if (this->_record) {
        Pa_StopStream(this->_record);
    }
}
