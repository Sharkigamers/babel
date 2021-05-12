/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** CServerClient
*/

#include "CServerClient.hpp"
#include "CServer.hpp"
#include <iostream>

babel::server::CServerClient::CServerClient(int id, const std::string &username, const std::string &password, bool isConnected)
    : CAUser(username, password), _id(id), _isConnected(isConnected), _tcpConnection(nullptr)
{
}

babel::server::CServerClient::CServerClient(std::shared_ptr<babel::server::CTcpConnection> &tcpConnection)
    : CAUser(), _id(NOT_CONNECTED_ID), _isConnected(true), _tcpConnection(tcpConnection)
{
    _isLogedIn = true;
}

babel::server::CServerClient::CServerClient()
    : CAUser(), _id(NOT_CONNECTED_ID), _isConnected(false), _tcpConnection(nullptr)
{
}

babel::server::CServerClient::CServerClient(const babel::server::CServerClient &other)
    : CAUser(other.getUsername(), other.getPassword())
{
    if (&other != this) {
        this->setId(other.getId());
        this->setTcpConnection(other.getTcpConnection());
    }
}

babel::server::CServerClient::~CServerClient()
{
}

std::string babel::server::CServerClient::toSQLString() const 
{
    std::string res = "";

    res.append("('" + getUsername() + "'," + "'" + getPassword() + "'" + ");");
    return res;
}

std::shared_ptr<babel::protocol::Message> babel::server::CServerClient::getLastMessage()
{
    boost::mutex mutex;

    mutex.lock();
    std::shared_ptr<babel::protocol::Message> lastMessage = _messageQueue[_messageQueue.size() - 1];
    
    _messageQueue.pop_back();
    mutex.unlock();
    return lastMessage;
}

std::string babel::server::CServerClient::getClientInfo() const
{
	return "";
}

void babel::server::CServerClient::authentificateClient(int id, const std::string &username, const std::string &password)
{
    setId(id);
    authentificate(username, password);
}

babel::server::CServerClient &babel::server::CServerClient::operator=(const CServerClient &other)
{
    if (&other != this) {

    }
    return *this;
}

void babel::server::CServerClient::readFromClient()
{
    (*getTcpConnection()->getSocket()).async_receive(boost::asio::buffer(_readBuffer, 4096), [this](const boost::system::error_code &error, std::size_t len) {
        if (!error) {
            babel::protocol::Message newMessage;
            std::cout << len << std::endl;

            if (len > 20) {
                try
                {
                    std::string header(_readBuffer, len);
                    std::string data = header.substr(8, len);
                    std::istringstream archive_stream(data);
                    boost::archive::text_iarchive archive(archive_stream);
                    archive >> newMessage;
                    _messageQueue.push_back(std::make_shared<babel::protocol::Message>(newMessage));
                    static_cast<babel::server::CServer *>(_server->getInstance())->handleNewMessage(this);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
            readFromClient();
        } else {
            std::cout << error.message() << std::endl;
            setIsLogedIn(false);
        }
    });
}