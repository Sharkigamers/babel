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
    (*getTcpConnection()->getSocket()).async_read_some(boost::asio::buffer(_readBufferHeader, 8), [this](const boost::system::error_code &error, std::size_t len) {
        if (!error) {
            try
            {
                std::string header(_readBufferHeader, len);
                std::size_t toRead = std::atof(header.c_str());
                readDataFromClient(error, toRead);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        } else {
            std::cout << (getUsername() == "" ? "Unconnected" : getUsername()) << " : Disconnected." << std::endl;
            setIsLogedIn(false);
        }
    });
}
void babel::server::CServerClient::readDataFromClient(const boost::system::error_code &error, const std::size_t toRead)
{
    (void)error;
	#if defined(_WIN32)
		char *buffer = new char[toRead];
	#else
		char buffer[toRead];
	#endif

    (*getTcpConnection()->getSocket()).async_read_some(boost::asio::buffer(buffer, toRead), [this, &buffer](const boost::system::error_code &newError, std::size_t len) {
        if (!newError) {
            babel::protocol::Message newMessage;
            
            std::string newBuffer(buffer, len);
            std::istringstream archive_stream(newBuffer);
            boost::archive::text_iarchive archive(archive_stream);
            archive >> newMessage;
            _messageQueue.push_back(std::make_shared<babel::protocol::Message>(newMessage));
            static_cast<babel::server::CServer *>(_server->getInstance())->handleNewMessage(this);
            readFromClient();
        } else {
            std::cout << newError.message() << std::endl;
            setIsLogedIn(false);
        }
    });
}