/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** Audio
*/

#ifndef CAUDIO_HPP_
#define CAUDIO_HPP_

#include <vector>

#include "IAudio.hpp"

namespace babel {
    namespace client {

        /* Error define */

        #define INITIALIZE_ERROR "Initialize portaudio failed"
        #define OPENSTREAM_ERROR "Opening a portaudio stream failed"
        #define STARTSTREAM_ERROR "Start a portaudio stream failed"
        #define STOPSTREAM_ERROR "Stop a portaudio stream failed"
        #define CLOSESTREAM_ERROR "Close a portaudio stream failed"
        #define TERMINATE_ERROR "Terminate portaudio failed"

        #define DEFAULT_INPUT_DEVICE_ERROR "No default input device has been found"

        #define WRITE_STREAM_ERROR "Write stream into buffer failed"
        #define READ_STREAM_ERROR "read stream from buffer failed"

        #define SUCCESS 0

        /* Constants variables define */

        #define CLIP_OFF paClipOff /* Disable default clipping of out of range samples */

        #define RECORD 1
        #define PLAY 2

        /* Audio class with portaudio */

        class CAudio : public IAudio {
            public:
                CAudio();
                ~CAudio();

                void initAudio() final;
                void startStream(char streamType) final;
                void stopStream(char streamType) final;
                void closeStream(char streamType) final;
                void terminateAudio() final;

                void setStreamParameters();

                void openRecordAudio();
                void openPlayAudio();

                PaStream *getRecStream() const { return _recordStream; };
                PaStream *getPlayStream() const { return _playStream; };

                short getChannelNumber() const { return _channelNumber; };
                long getSampleRate() const { return _sampleRate; };
                int getFramePerBuffer() const { return _framePerBuffer; };

                void setChannelNumber(short channelNumber) { _channelNumber = channelNumber; };
                void setSample(int sampleRate) { _sampleRate = sampleRate; };
                void setFramePerBuffer(long framePerBuffer) { _framePerBuffer = framePerBuffer; };

                std::vector<uint32_t> recordAudio();
                void playAudio(std::vector<uint32_t> writeableBuffer);

            protected:
            private:
                PaError _error;
                PaStreamParameters _paramOutput;
                PaStreamParameters _paramInput;
                PaStream *_recordStream;
                PaStream *_playStream;

                bool _recordRunning = false;
                bool _playRunning = false;

                short _channelNumber = 1; /* one for output and one for input */
                int _sampleRate = 48000; /* 41000 for low sample rate but can generate warnings (not stable ) */
                long _framePerBuffer = 480; /* Not too small for not sending too much packets */
        };

        /* Error Audio class */

        class CAudioError : public std::exception {
            public:
                CAudioError(const std::string &);
                ~CAudioError();

                const char *what() const throw() { return _errorMessage.c_str(); }

            private:
                std::string _errorMessage;
        };
    }
}

#endif /* !CAUDIO_HPP_ */
