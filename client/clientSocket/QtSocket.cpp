/*
** EPITECH PROJECT, 2020
** QtSocket.cpp
** File description:
** QtSocket
*/

#include "QtSocket.hpp"

babel::client::QtSocket::QtSocket(const QString &hostName, quint16 port) : 
_socket(std::make_unique<QTcpSocket>())
{
    _socket->connectToHost(hostName, port);
}

babel::client::QtSocket::~QtSocket()
{
}

void babel::client::QtSocket::readFromServer()
{
    _socket->waitForReadyRead(3000);
    qDebug() << _socket->readAll();
}
