/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CTcpConnection
*/

#include "CTcpConnection.hpp"

babel::server::CTcpConnection::CTcpConnection(boost::asio::io_service &ioService)
    : _socket(std::make_unique<boost::asio::ip::tcp::socket>(ioService))
{
}

babel::server::CTcpConnection::~CTcpConnection()
{
}
