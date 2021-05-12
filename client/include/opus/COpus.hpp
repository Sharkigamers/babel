/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** Opus
*/

#ifndef COPUS_HPP_
#define COPUS_HPP_

#include <vector>

#include "opus.h"

#include "IOpus.hpp"

namespace babel {
    namespace client {

        class COpus : public IOpus {
            public:

                #define VOIP_FLAG OPUS_APPLICATION_VOIP

            public:
                COpus(opus_int32 samplingRate, int channelNumber);
                ~COpus();

                void create_encoder() final;
                void create_decoder() final;

                void destroy_encoder() final;
                void destroy_decoder() final;

                std::vector<uint32_t> encode(std::vector<uint32_t> encodableBuffer) final;
                std::vector<uint32_t> decode(std::vector<uint32_t> encoded) final;

            protected:
            private:
                OpusEncoder *_encoder;
                OpusDecoder *_decoder;

                opus_int32 _samplingRate; /* Usually sampling rate is 48000 (41000 is not stable) */
                short _channel; /* 1 channels generally */

                int _error;
        };


        /* Error Opus class */

        class COpusError : public std::exception {
            public:

                #define ENCODE_CREATION_ERROR "Encode creation from opus failed"
                #define DECODE_CREATION_ERROR "Decode creation from opus failed"
                #define ENCODING_ERROR "Encoding sample from opus failed"
                #define DECODING_ERROR "Decoding sample from opus failed"

            public:
                COpusError(const std::string &);
                ~COpusError();

                const char *what() const throw() { return _errorMessage.c_str(); }

            private:
                std::string _errorMessage;
        };
    }
}

#endif /* !COPUS_HPP_ */
