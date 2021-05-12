/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CCallManager
*/

#ifndef CCALLMANAGER_HPP_
#define CCALLMANAGER_HPP_

#include <memory>
#include "JsonUtility.hpp"
#include "AProtocol.hpp"
#include "QtDataSocket.hpp"
#include "CAudio.hpp"
#include "COpus.hpp"
#include <boost/thread.hpp>

namespace babel {
    namespace client {
        class CCallManager {
            public:
                CCallManager(const std::string &host);
                ~CCallManager();

                void setPort(unsigned short port) { _serverSocket->bind(_host, port); };

                void initCall();
                void initAudio();
                void startCall(const babel::protocol::Message &);
                void endCall();
                bool isInCall() const { return _inCall; }
                int partnerId() const noexcept { return _partnerId; }
                void partnerId(int id) noexcept { _partnerId = id; }

                void setNewHostPortWrite(const std::string &, unsigned short);
                std::map<std::string, std::string> getContactInfo() const;

            protected:
            private:
                void loop();
                std::string _host;
                std::unique_ptr<boost::thread> _audioThread;
                std::unique_ptr<babel::client::QtDataSocket> _serverSocket;
                std::unique_ptr<babel::client::CAudio> _audioInterface;
                std::unique_ptr<babel::client::COpus> _encoder;
                bool _inCall;
                bool _bound;
                int _partnerId;
        };
    }
}

#endif /* !CCALLMANAGER_HPP_ */
