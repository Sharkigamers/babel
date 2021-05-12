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
    try
    {
        _socket->connectToHost(hostName, port);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

babel::client::QtSocket::~QtSocket()
{
}

std::shared_ptr<babel::protocol::Message> babel::client::QtSocket::readFromServer()
{
    try
    {
        _socket->waitForReadyRead(3000);
        QByteArray qBuffer = _socket->read(8);
        babel::protocol::Message newMessage;
        
        std::string stdBuffer = qBuffer.toStdString();
        if (stdBuffer == "") {
            return std::make_shared<babel::protocol::Message>();
        }
        qint32 size = std::atoi(stdBuffer.c_str());
        qBuffer = _socket->read(size);
        stdBuffer = qBuffer.toStdString();
        std::istringstream archive_stream(stdBuffer);
        boost::archive::text_iarchive archive(archive_stream);
        archive >> newMessage;
        // std::cout << newMessage.getMessageType() << std::endl;
        return (std::make_unique<babel::protocol::Message>(newMessage));
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return std::make_shared<babel::protocol::Message>();
}
