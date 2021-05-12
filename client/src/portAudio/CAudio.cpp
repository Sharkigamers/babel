/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** Audio
*/

#include <stdlib.h>
#include <string.h>

#include "CAudio.hpp"

babel::client::CAudio::CAudio()
: _recordStream(nullptr), _playStream(nullptr)
{
    initAudio();
    setStreamParameters();
}

babel::client::CAudio::~CAudio()
{
    stopStream(RECORD);
    stopStream(PLAY);
    closeStream(RECORD);
    closeStream(PLAY);
    terminateAudio();
}


/* Basics of portaudio */

void babel::client::CAudio::initAudio()
{
    _error = Pa_Initialize();
    
    if (_error != paNoError)
        throw babel::client::CAudioError(INITIALIZE_ERROR);
}

void babel::client::CAudio::startStream(char streamType)
{
    if (streamType == RECORD) {
        _error = Pa_StartStream(_recordStream);

        if (_error != paNoError)
            throw babel::client::CAudioError(STARTSTREAM_ERROR);

        _recordRunning = true;

    } else if (streamType == PLAY) {
        _error = Pa_StartStream(_playStream);
            
        if (_error != paNoError)
            throw babel::client::CAudioError(STARTSTREAM_ERROR);
        _playRunning = true;
    }
}

void babel::client::CAudio::stopStream(char streamType)
{
    if (streamType == RECORD && _recordRunning) {
        _error = Pa_StopStream(_recordStream);
            
        if (_error != paNoError)
            throw babel::client::CAudioError(STOPSTREAM_ERROR);
        _recordRunning = false;
    } else if (streamType == PLAY && _playRunning) {
        _error = Pa_StopStream(_playStream);
            
        if (_error != paNoError)
            throw babel::client::CAudioError(STOPSTREAM_ERROR);
        _playRunning = false;
    }
}

void babel::client::CAudio::closeStream(char streamType)
{
    if (streamType == RECORD) {
        _error = Pa_CloseStream(_recordStream);
            
        if (_error != paNoError)
            throw babel::client::CAudioError(CLOSESTREAM_ERROR);
        _recordRunning = false;
    } else if (streamType == PLAY) {
        _error = Pa_CloseStream(_playStream);
            
        if (_error != paNoError)
            throw babel::client::CAudioError(CLOSESTREAM_ERROR);
        _playRunning = false;
    }
}

void babel::client::CAudio::terminateAudio()
{
    _error = Pa_Terminate();
        
    if (_error != paNoError)
        throw babel::client::CAudioError(TERMINATE_ERROR);
}


/* Recording */

void babel::client::CAudio::setStreamParameters()
{
    _paramInput.device = Pa_GetDefaultInputDevice();
    _paramOutput.device = Pa_GetDefaultOutputDevice();

    if (_paramOutput.device == paNoDevice || _paramInput.device == paNoDevice)
        throw babel::client::CAudioError(DEFAULT_INPUT_DEVICE_ERROR);

    _paramInput.channelCount = _channelNumber;
    _paramOutput.channelCount = _channelNumber;

    _paramInput.sampleFormat = paInt16; /* need to adapt buffer */
    _paramOutput.sampleFormat = paInt16; /* need to adapt buffer */

    _paramInput.suggestedLatency = Pa_GetDeviceInfo(_paramInput.device)->defaultHighInputLatency;
    _paramOutput.suggestedLatency = Pa_GetDeviceInfo(_paramOutput.device)->defaultHighOutputLatency;

    _paramOutput.hostApiSpecificStreamInfo = NULL;
    _paramInput.hostApiSpecificStreamInfo = NULL;
}

void babel::client::CAudio::openRecordAudio()
{
    if (!_recordRunning) {
        _error = Pa_OpenStream(
            &_recordStream,
            &_paramInput,
            NULL, /* for opening on input */
            _sampleRate,
            _framePerBuffer,
            CLIP_OFF,
            NULL,
            NULL
        );
        if (_error != paNoError)
            throw babel::client::CAudioError(OPENSTREAM_ERROR);
    }
}

void babel::client::CAudio::openPlayAudio()
{
    if (!_playRunning) {
        _error = Pa_OpenStream(
            &_playStream,
            NULL, /* for opening on output */
            &_paramOutput,
            _sampleRate,
            _framePerBuffer,
            CLIP_OFF,
            NULL,
            NULL
        );

        if (_error != paNoError)
            throw babel::client::CAudioError(OPENSTREAM_ERROR);
    }
}

std::vector<uint16_t> babel::client::CAudio::recordAudio()
{
    std::vector<uint16_t> readableBuffer(_framePerBuffer);

    /* to not read to a too hight size we check the readable size */
    signed long readableSize = Pa_GetStreamReadAvailable(_recordStream);

    if (readableSize < _framePerBuffer)
        _error = Pa_ReadStream(
            _recordStream,
            readableBuffer.data(),
            readableSize
        );
    else
        _error = Pa_ReadStream(
            _recordStream,
            readableBuffer.data(),
            _framePerBuffer
        );

    if (_error != paNoError)
        throw babel::client::CAudioError(READ_STREAM_ERROR);
    
    return readableBuffer;
}

void babel::client::CAudio::playAudio(std::vector<uint16_t> writeableBuffer)
{
    /* if you do no wait enought it will crash */
    // while (Pa_GetStreamWriteAvailable(_playStream) < (signed long)writeableBuffer.size());

    _error = Pa_WriteStream(
        _playStream,
        writeableBuffer.data(),
        (unsigned long)writeableBuffer.size()
    );

    if (_error != paNoError)
        throw babel::client::CAudioError(WRITE_STREAM_ERROR);
}


/* Class for error gestion */

babel::client::CAudioError::CAudioError(const std::string &message)
: _errorMessage(message)
{
}

babel::client::CAudioError::~CAudioError()
{

}
