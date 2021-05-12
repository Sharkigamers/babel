/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CTcpServer
*/

#include "CTcpServer.hpp"

#include <iostream>
#include <ios>

#include "CServer.hpp"

babel::server::CTcpServer::CTcpServer(bool isIPv4, unsigned short port, babel::server::IServer *server)
    : _ioService(), _server(server), _clientWatcherThread([this]() {
        boost::mutex mutex;
        while (1) {
			boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
            mutex.lock();
            std::cout << "Nb clients: " << _clientsQueue.size() << std::endl;
            for (std::size_t i = 0; i < _clientsQueue.size(); i++) {
                if (_clientsQueue[i]->isLogedIn() == false) {
                    static_cast<babel::server::CServer *>(_server)->onClientDisconnect(*_clientsQueue.at(i));
                    _clientsQueue.erase(_clientsQueue.begin() + i);
                    continue;
                }
            }
            mutex.unlock();
        }
    })
{
    auto x = [isIPv4]() {
        if (isIPv4) {
            return boost::asio::ip::tcp::v4();
        } else {
            return boost::asio::ip::tcp::v6();
        }
    };
    boost::asio::ip::tcp::endpoint endpoint(x(), port);
    _acceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(getIoService(), endpoint);

    std::cout << "Listening on port " << port << '\n';
}

babel::server::CTcpServer::~CTcpServer()
{
}

void babel::server::CTcpServer::waitClient()
{
    std::shared_ptr<babel::server::CTcpConnection> newConnection = std::make_shared<babel::server::CTcpConnection>(getIoService());

    _acceptor->async_accept(*newConnection->getSocket(),
        std::bind(&babel::server::CTcpServer::acceptConnection, this,
        std::placeholders::_1,
        newConnection)
    );
}

void babel::server::CTcpServer::acceptConnection(const boost::system::error_code& error, std::shared_ptr<babel::server::CTcpConnection> newConnection)
{
    if (!error)
    {
        std::cout << "Connection accepted from " << newConnection->getSocket()->remote_endpoint().address().to_string() << std::endl;
        _clientsQueue.push_back(std::make_shared<babel::server::CServerClient>(newConnection));
        _clientsQueue[_clientsQueue.size() -1]->setServer(_server);
        _clientsQueue[_clientsQueue.size() -1]->readFromClient();
        this->waitClient();
	}
	else {
		std::cerr << "Connection denied" << std::endl;
	}
}