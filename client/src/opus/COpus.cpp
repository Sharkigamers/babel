/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** COpus
*/

#include <iostream>

#include "COpus.hpp"

babel::client::COpus::COpus(opus_int32 samplingRate, int channelNumber)
: _samplingRate(samplingRate), _channel(channelNumber)
{
    create_encoder();
    create_decoder();

    opus_encoder_ctl(_encoder, OPUS_GET_BANDWIDTH(&_encodedSize));
}

babel::client::COpus::~COpus()
{
    destroy_encoder();
    destroy_decoder();
}

void babel::client::COpus::create_encoder()
{
    _encoder = opus_encoder_create(
        _samplingRate,
        (int)_channel,
        VOIP_FLAG, /* specific flag for VOIP */
        &_error
    );
    
    if (_error != OPUS_OK)
        throw babel::client::COpusError(ENCODE_CREATION_ERROR);
}

void babel::client::COpus::create_decoder()
{
    _decoder = opus_decoder_create(
        _samplingRate,
        (int)_channel,
        &_error
    );

    if (_error != OPUS_OK)
        throw babel::client::COpusError(DECODE_CREATION_ERROR);
}

void babel::client::COpus::destroy_encoder()
{
    opus_encoder_destroy(_encoder);
}

void babel::client::COpus::destroy_decoder()
{
    opus_decoder_destroy(_decoder);
}

std::vector<uint16_t> babel::client::COpus::encode(const std::vector<uint16_t> &encodableBuffer)
{
    /*
        We don't want modify our output buffer size 
    
        Accepted:
            opus_int16: short
        
        Portaudio needs to send a vector uint16_t for encoding sample
    */

    std::vector<uint16_t> decodableBuffer(_encodedSize);

    int bytesDataPlayload = opus_encode(
        _encoder,
        (const opus_int16 *)encodableBuffer.data(),
        (int)encodableBuffer.size(),
        (unsigned char *)decodableBuffer.data(),
        (opus_int32)decodableBuffer.size()
    );

    if (bytesDataPlayload < OPUS_OK)
        throw babel::client::COpusError(ENCODING_ERROR);
    
    return decodableBuffer;
}

std::vector<uint16_t> babel::client::COpus::decode(const std::vector<uint16_t> &encoded)
{
    std::vector<uint16_t> decodedBuffer(480);

    int bytesDecodedSample = opus_decode(
        _decoder,
        (const unsigned char *)encoded.data(),
        _encodedSize,
        (opus_int16 *)decodedBuffer.data(),
        (int)decodedBuffer.size(),
        0 /* request that any in-band forward error correction data be decoded */
    );

    if (bytesDecodedSample < OPUS_OK)
        throw babel::client::COpusError(DECODING_ERROR);
    return decodedBuffer;
}


/* Class for error gestion */

babel::client::COpusError::COpusError(const std::string &message)
: _errorMessage(message)
{
}

babel::client::COpusError::~COpusError()
{

}
