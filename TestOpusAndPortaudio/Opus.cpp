/*
** EPITECH PROJECT, 2019
** opus
** File description:
** opus
*/

#include "Opus.hpp"

Opus::Opus(void) // init opus
{
    int decoderError = 0;
    int encoderError = 0;
    bool okCreate = true;

    this->_decoder = opus_decoder_create(SAMPLE_RATE, 1, &decoderError);
    this->_encoder = opus_encoder_create(SAMPLE_RATE, 1, APPLICATION, &encoderError);

    if (!this->_decoder) {
        std::cerr << "Can not create opus decoder: " << decoderError << std::endl;
        okCreate = false;
    }
    if (!this->_encoder) {
        std::cerr << "Can not create opus encoder: " << encoderError << std::endl;
        okCreate = false;
    }
    if (!okCreate) {
        throw std::exception();
    }
    decoderError = opus_decoder_init(this->_decoder, SAMPLE_RATE, 1);
    encoderError = opus_encoder_init(this->_encoder, SAMPLE_RATE, 1, APPLICATION);
    if (decoderError != OPUS_OK) {
        throw std::exception();
    }
    if (encoderError != OPUS_OK) {
        throw std::exception();
    }
    encoderError = opus_encoder_ctl (this->_encoder, OPUS_SET_BITRATE(BITRATE));
    if (encoderError != OPUS_OK) {
        std::cerr << "Error set bitrate encoder";
        throw std::exception();
    }
}

Opus::~Opus() // destroy opus
{
    if (this->_decoder) {
        opus_decoder_destroy(this->_decoder);
    }
    if (this->_encoder) {
        opus_encoder_destroy(this->_encoder);
    }
}
