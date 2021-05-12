/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** QtDataSocket
*/

#ifndef QtDataSocket_HPP_
#define QtDataSocket_HPP_

#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <string>
#include <QUdpSocket>
#include <QtNetwork>
#include <QDebug>
#include <QNetworkDatagram>

#include <QtNetwork/QUdpSocket>

#include <vector>
#include <array>

#include "IAudio.hpp"

namespace babel {
    namespace client {

        enum Mode {
            READ,
            WRITE
        };

        class QtDataSocket {
            public:
                QtDataSocket(const std::string &, unsigned short, const Mode &);
                QtDataSocket(const Mode &);
                ~QtDataSocket();

                void bind(const std::string &, unsigned short);
                void unbind();
                Mode getMode() const { return _mode; }
                void setMode(const Mode &mode) { _mode = mode; }
                
                void setWriteHost(const std::string &host) { _writeHost = host; }
                void setWritePort(const unsigned short &port) { _writePort = port; }
                std::string getWriteHost() const { return _host; }
                unsigned short getWritePort() const { return _port; }

                void sendData(const std::vector<uint16_t> &) const;
                std::vector<uint16_t> getData();
            protected:
            private:
                std::unique_ptr<boost::asio::ip::udp::socket> _socket;
                bool _isConnected;
                Mode _mode;
                boost::asio::ip::udp::endpoint _toSendEndpoint;
                std::string _host;
                std::string _writeHost;
                unsigned short _port;
                unsigned short _writePort;
                boost::asio::io_service _ioService;
                std::vector<std::vector<uint16_t> > _dataPackets;
        };
    }
}

#endif /* !QtDataSocket_HPP_ */
