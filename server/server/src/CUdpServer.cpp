/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CUdpServer
*/

#include "CUdpServer.hpp"

babel::server::CUdpServer::CUdpServer(const std::string &host, unsigned short port, int id, const std::vector<babel::server::CUdpClient> &clients)
    : _ioService(), _endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)), _socket(_ioService, _endpoint), _clients(clients), _id(id)
{
    (void)host;
    std::cout << "Udp server listening on port : " << port << ".\n";
    // _ioService.run();
    // _ioService.post()
}

babel::server::CUdpServer::CUdpServer(const std::string &host, unsigned short port, int id, const babel::server::CUdpClient &client)
    : _ioService(), _endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)), _socket(_ioService), _id(id)
{
    (void)host;
    _socket.open(_endpoint.protocol());
    _socket.bind(_endpoint);
    std::cout << "Udp server listening on port : " << port << ".\n";
    _clients.push_back(client);
}

babel::server::CUdpServer::CUdpServer(const std::string &host, unsigned short port, int id)
    : _ioService(), _endpoint(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)), _socket(_ioService, _endpoint), _id(id)
{
    (void)host;
    std::cout << "Udp server listening on port : " << port << ".\n";
    // _ioService.run();
}

babel::server::CUdpServer::CUdpServer(const CUdpServer &other)
    : _ioService(), _socket(_ioService, other.getEndpoint())
{
    if (&other != this) {
        _endpoint = other.getEndpoint();
        _clients = other.getClients();
    }
    // _ioService.run();
}

babel::server::CUdpServer::~CUdpServer()
{
    std::cout << "Udp server listening on port: " << _endpoint.port() << " shutdown.\n"; 
    _socket.close();
}

babel::server::CUdpServer &babel::server::CUdpServer::operator=(const CUdpServer &other)
{
    if (&other != this) {
        _endpoint = other.getEndpoint();
        _clients = other.getClients();
    }
    std::cout << "??" << std::endl;
    return *this;
}

// void babel::server::CUdpServer::startReceive()
// {
//     _socket.async_receive_from(boost::asio::buffer(_buffer, 960), _senderEndpoint, [this](const boost::system::error_code &error, std::size_t len) {
//         if (!error) {
//             bool toFind = true;
//             for (const auto &x : _clientEndpoint) {
//                 if (*x == _senderEndpoint) 
//                     toFind = false;
//             }
//             if (toFind)
//                 _clientEndpoint.push_back(std::make_shared<boost::asio::ip::udp::endpoint>(_senderEndpoint));
//             for (const auto &x : _clientEndpoint) {
//                 if (*x != _senderEndpoint) {
//                     _socket.async_send_to(boost::asio::buffer(_buffer, 960), *x, [this](const boost::system::error_code &error_write, std::size_t bytes_transferred){
//                         if (error_write)
//                         {
//                             std::cout << error_write.message() << std::endl;
//                         } else {
//                             std::cout << "Sent " << bytes_transferred << " bytes to " << _senderEndpoint.address().to_string() << " : " << _senderEndpoint.port() << std::endl;
//                         }
//                     });
//                 }
//             }
//             startReceive();
//         } else {
//             std::cout << error.message() << std::endl;
//         }
//     });
// }

void babel::server::CUdpServer::startReceive()
{
    _running = true;
    try
    {
        boost::mutex mutex;
        while (_running) {
            boost::system::error_code error;
            boost::asio::ip::udp::endpoint senderEndpoint;
            std::array<char, 2210> buffers;

            try
            {
                _socket.receive_from(boost::asio::buffer(buffers), senderEndpoint, 0, error);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            std::cout << "Received from " << senderEndpoint.address().to_string() << std::endl;

            // std::cout << "From: " << senderEndpoint.address().to_string() << " : " << senderEndpoint.port() << std::endl;

            // if (error && error != boost::asio::error::message_size)
            //     throw boost::system::system_error(error);
            bool add = true;
            mutex.lock();
            for (const auto &x: _clientEndpoint) {
                if (senderEndpoint.address() == x->address()) add = false;
            }
            if (add)
                _clientEndpoint.push_back(std::make_shared<boost::asio::ip::udp::endpoint>(boost::asio::ip::make_address(senderEndpoint.address().to_string()), senderEndpoint.port()));
            mutex.unlock();

            _packets.push_back(std::make_pair(senderEndpoint, buffers));

            // for (const auto &x : _clients) {
            //     // if (x.getEndpoint() != senderEndpoint) {
            //         _socket.send_to(boost::asio::buffer(buffers), _senderEndpoint, 0, error);
            //     // }
            // }

        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

}

void babel::server::CUdpServer::startWrite()
{
    _running = true;
    try
    {
        boost::mutex mutex;
        while (_running) {
			boost::this_thread::sleep_for(boost::chrono::milliseconds(200));
            // mutex.lock();
            std::cout << "Packets size: " << _packets.size() << std::endl;
            for (const auto &packet : _packets) {
                for (const auto &x: _clientEndpoint) {
                    if (x->address().to_string() != packet.first.address().to_string() && x->port() != packet.first.port()) {
                        _socket.send_to(boost::asio::buffer(packet.second), *x, 0);
                    }
                }
            }
            _packets.clear();
            // mutex.unlock();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}