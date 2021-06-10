/*
** EPITECH PROJECT, 2019
** opus
** File description:
** opus
*/

#pragma once

#include <opus/opus.h>
#include <opus/opus_types.h>

#include <thread>
#include <iostream>
#include <vector>

#define SAMPLE_RATE         48000
#define BITRATE             16000
#define APPLICATION         OPUS_APPLICATION_AUDIO

class Opus
{
private:
public:
    OpusDecoder *_decoder = nullptr;
    OpusEncoder *_encoder = nullptr;
    Opus();
    ~Opus();
};
