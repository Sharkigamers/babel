/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** TCPServer
*/

#ifndef TCPSERVER_HPP_
#define TCPSERVER_HPP_

#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/core/ref.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <array>
#include <tuple>

#include "IServer.hpp"
#include "CTcpConnection.hpp"
#include "CServerClient.hpp"
#include "AProtocol.hpp"

namespace babel {
    namespace server {
        
        class CTcpServer {
            public:
                CTcpServer(bool isIpV4, unsigned short port, babel::server::IServer *server);
                ~CTcpServer();

                void waitClient();

                boost::asio::io_service& getIoService() { return _ioService; }

                bool isNewClientAvailable() const { return !_clientsQueue.empty(); }

                std::shared_ptr<babel::server::CServerClient> getLastClient()
                {
                    if (_clientsQueue.empty() == true) {
                        return nullptr;
                    } else {
                        std::shared_ptr<babel::server::CServerClient> res = _clientsQueue[_clientsQueue.size() - 1];
                        _clientsQueue.pop_back();
                        res->readFromClient();
                        return res;
                    }
                };

                template <typename T, typename Handler>
                void writeToClient(babel::server::CServerClient &client, const T &t, Handler handler)
                {
                    std::ostringstream archive_stream;
                    boost::archive::text_oarchive archive(archive_stream);
                    archive << t;
                    std::string m_outbound_data = archive_stream.str();

                    std::ostringstream header_stream;
                    header_stream << std::setw(8)
                        << std::hex << m_outbound_data.size();
                    if (!header_stream || header_stream.str().size() != 8)
                    {
                        std::cout << "RIP" << std::endl;
                        return;
                    }
                    std::string m_outbound_header = header_stream.str();

                    std::vector<boost::asio::const_buffer> buffers;
                    buffers.push_back(boost::asio::buffer(m_outbound_header));
                    buffers.push_back(boost::asio::buffer(m_outbound_data));
                    boost::asio::async_write(*client.getTcpConnection()->getSocket(), buffers, handler);
                }

            protected:
            private:
                void acceptConnection(const boost::system::error_code& error, std::shared_ptr<babel::server::CTcpConnection>); // (3)
                boost::asio::io_service _ioService;
                std::shared_ptr<boost::asio::ip::tcp::acceptor> _acceptor;
                std::vector<std::shared_ptr<babel::server::CServerClient> > _clientsQueue;
                babel::server::IServer *_server;
        };
    }
}

#endif /* !TCPSERVER_HPP_ */
