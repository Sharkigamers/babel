/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CCallManager
*/

#include "CCallManager.hpp"

babel::client::CCallManager::CCallManager(const std::string &host)
    :  _host(host), _inCall(false), _bound(false)
{
    _audioInterface = std::make_unique<babel::client::CAudio>();
    _encoder = std::make_unique<babel::client::COpus>(_audioInterface->getSampleRate(), _audioInterface->getChannelNumber());
    _serverSocket = std::make_unique<babel::client::QtDataSocket>(READ);
}

babel::client::CCallManager::~CCallManager()
{
}

void babel::client::CCallManager::initAudio()
{}

void babel::client::CCallManager::setNewHostPortWrite(const std::string &newHost, unsigned short newPort)
{
    (void)newHost;
    _serverSocket->bind(_host, newPort);
    _bound = true;
}

void babel::client::CCallManager::initCall()
{
    _inCall = true;
    _audioInterface->openRecordAudio();
    _audioInterface->openPlayAudio();
    _audioInterface->startStream(RECORD);
    _audioInterface->startStream(PLAY);
    _audioThread.reset(new boost::thread([this]() {
        loop();
    }));
}

void babel::client::CCallManager::startCall(const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    const unsigned short port = static_cast<unsigned short>(std::stoi(objects.at("Port")));

    _serverSocket->bind(_host, port);
    _bound = true;
    _inCall = true;
    _audioInterface->openRecordAudio();
    _audioInterface->openPlayAudio();
    _audioInterface->startStream(RECORD);
    _audioInterface->startStream(PLAY);
    _audioThread.reset(new boost::thread([this]() {
        loop();
    }));
}

void babel::client::CCallManager::endCall()
{
    _serverSocket->unbind();
    _bound = false;
    _inCall = false;
    _audioThread->interrupt();
    _audioThread.reset(nullptr);
}

std::map<std::string, std::string> babel::client::CCallManager::getContactInfo() const
{
    std::map<std::string, std::string> info;
    info["Host"] = _serverSocket->getWriteHost();
    info["Port"] = std::to_string(_serverSocket->getWritePort());

	return info;
}

void babel::client::CCallManager::loop()
{
    const auto sendAudioLambda = [this]() {
        const auto audioBuffer =  _encoder->encode(_audioInterface->recordAudio());

        _serverSocket->sendData(audioBuffer);
    };

    const auto getAudioLambda = [this]() {
        const auto audioBuffer =  _serverSocket->getData();

        if (!audioBuffer.empty())
            _audioInterface->playAudio(_encoder->decode(audioBuffer));
    };
    
    boost::mutex mutex;
    while (isInCall()) {
        mutex.lock();
        try
        {
            if (_bound) {
                sendAudioLambda();
                getAudioLambda();
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        mutex.unlock();
    }
}