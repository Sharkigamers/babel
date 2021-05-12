/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CUdpClient
*/

#include "CUdpClient.hpp"

babel::server::CUdpClient::CUdpClient(const std::string &addr, unsigned short port)
    : _clientEndpoint(boost::asio::ip::address::from_string(addr), port)
{
}

babel::server::CUdpClient::~CUdpClient()
{
}
