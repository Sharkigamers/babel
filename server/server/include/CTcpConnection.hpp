/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CTcpConnection
*/

#ifndef CTCPCONNECTION_HPP_
#define CTCPCONNECTION_HPP_

#include <boost/asio.hpp>
#include <memory>

namespace babel {
    namespace server {
        class CTcpConnection {
            public:
                CTcpConnection(boost::asio::io_service &ioService);
                ~CTcpConnection();

                std::unique_ptr<boost::asio::ip::tcp::socket> &getSocket() { return _socket; }

            protected:
            private:
                std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
        };
    }
}

#endif /* !CTCPCONNECTION_HPP_ */
