/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** IAudio
*/

#ifndef IAUDIO_HPP_
#define IAUDIO_HPP_

#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include "portaudio.h"

inline std::string convertAudioToString(const std::vector<uint16_t> &buffer)
{
    std::string res;

    for (const auto &x : buffer) {
        res += std::to_string(x) + ";";
    }
    return res;
}

inline std::vector<uint16_t> convertStringToAudio(std::string str)
{
    size_t pos = 0;
    std::vector<uint16_t> res;
    std::string token;
    const std::string delim = ";";

    while ((pos = str.find(delim)) != std::string::npos) {
        token = str.substr(0, pos);
        try
        {
            res.push_back(static_cast<uint16_t>(std::atoi(token.c_str())));
        }
        catch(const std::exception& e)
        {}        
        str.erase(0, pos + delim.length());
    }
    return res;
}

namespace babel {
    namespace client {

        #define RECORD 1
        #define PLAY 2

        class IAudio {
            public:
                IAudio() {};

                virtual void initAudio() = 0;
                virtual void startStream(char streamType) = 0;
                virtual void stopStream(char streamType) = 0;
                virtual void closeStream(char streamType) = 0;
                virtual void terminateAudio() = 0;

            protected:
            private:

        };
    }
}

#endif /* !IAUDIO_HPP_ */
