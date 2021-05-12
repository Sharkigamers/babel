/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CUdpSocket
*/

#ifndef CUDPSERVER_HPP_
#define CUDPSERVER_HPP_

#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/core/ref.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/tuple/tuple.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <array>
#include <tuple>

#include "CUdpClient.hpp"

namespace babel {
    namespace server {
        class CUdpServer {
            public:
                CUdpServer(const std::string &, unsigned short, int, const std::vector<babel::server::CUdpClient> &);
                CUdpServer(const std::string &, unsigned short, int, const babel::server::CUdpClient &);
                CUdpServer(const std::string &, unsigned short, int);
                CUdpServer(const CUdpServer &other);
                ~CUdpServer();

                int getId() const noexcept { return _id; }
                boost::asio::io_service &getIoService() noexcept { return _ioService;}
                boost::asio::ip::udp::endpoint getEndpoint() const noexcept { return _endpoint; }
                boost::asio::ip::udp::socket &getSocket() noexcept { return _socket; }
                std::vector<babel::server::CUdpClient> getClients() const noexcept { return _clients; }
                unsigned short getPort() const noexcept { return _endpoint.port(); }

                void stop() { _running = false; };
                bool operator==(const CUdpServer &other) { return _id == other.getId(); }
                bool operator==(const int id) { return _id == id; }
                CUdpServer &operator=(const CUdpServer &other);
                CUdpServer &operator=(CUdpServer &&other)
                {
                    (void)other;
                    return *this;
                };
                void addClient(const babel::server::CUdpClient &client) { _clients.push_back(client); }

                void startReceive();
                void startWrite();

            private:
                boost::asio::io_service _ioService;
                boost::asio::ip::udp::endpoint _endpoint;
                boost::asio::ip::udp::socket _socket;
                std::vector<babel::server::CUdpClient> _clients;
                std::vector<std::shared_ptr<boost::asio::ip::udp::endpoint> > _clientEndpoint;
                int _id;
                char _buffer[2210];
                std::vector<std::pair<boost::asio::ip::udp::endpoint, std::array<char, 2210> > > _packets;
                bool _running;

            private:
                boost::asio::ip::udp::endpoint _senderEndpoint;

        };
    }
}

#endif /* !CUDPSERVER_HPP_ */
