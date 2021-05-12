/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** IAudio
*/

#ifndef IAUDIO_HPP_
#define IAUDIO_HPP_

#include <memory>

#include "portaudio.h"

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
