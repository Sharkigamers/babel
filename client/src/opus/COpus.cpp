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

std::vector<uint32_t> babel::client::COpus::encode(std::vector<uint32_t> encodableBuffer)
{
    /*
        We don't want modify our output buffer size 
    
        Accepted:
            opus_int16: short
        
        Portaudio needs to send a vector uint32_t for encoding sample
    */

   std::cout << "ENTER !!!!\n";

    std::vector<uint32_t> decodableBuffer(encodableBuffer.size());

    std::cout << "ENCODE \n";

    for (int i = 0; i < encodableBuffer.size(); ++i)
        std::cout << encodableBuffer[i] << ",";

    std::cout << "\n\n";

    int bytesDataPlayload = opus_encode(
        _encoder,
        (const opus_int16 *)encodableBuffer.data(),
        (int)encodableBuffer.size(),
        (unsigned char *)decodableBuffer.data(),
        (opus_int32)decodableBuffer.size()
    );

    std::cout << "\nLA0\n\n";

    if (bytesDataPlayload < OPUS_OK)
        throw babel::client::COpusError(ENCODING_ERROR);
    
    for (int i = 0; i < decodableBuffer.size(); ++i)
        std::cout << decodableBuffer[i] << ",";

    std::cout << "PDPDPD0\n";
    
    return (std::vector<uint32_t>)decodableBuffer;
}

std::vector<uint32_t> babel::client::COpus::decode(std::vector<uint32_t> encoded)
{
    /*
        We don't want modify our output buffer size 
    
        Accepted:
            opus_int16: short

        Portaudio needs to receive a vector uint32_t of decoded sample
    */


    std::cout << "DECODE \n";
    std::vector<uint32_t> decodedBuffer(encoded.size());

    int bytesDecodedSample = opus_decode(
        _decoder,
        (const unsigned char *)encoded.data(),
        (opus_int32)encoded.size(),
        (opus_int16 *)decodedBuffer.data(),
        (int)decodedBuffer.size(),
        0 /* request that any in-band forward error correction data be decoded */
    );

    for (int i = 0; i < decodedBuffer.size(); ++i)
        std::cout << decodedBuffer[i] << ",";

    std::cout << "\nLA\n";

    if (bytesDecodedSample < OPUS_OK)
        throw babel::client::COpusError(DECODING_ERROR);
    std::cout << "PDPDPD\n";
    
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
