/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** QtDataSocket
*/

#include "QtDataSocket.hpp"

babel::client::QtDataSocket::QtDataSocket(const std::string &host, unsigned short port, const Mode &mode)
    : _mode(mode), _host(host), _port(port), _ioService()
{
    _socket = std::make_unique<boost::asio::ip::udp::socket>(_ioService);
    _socket->open(boost::asio::ip::udp::v4());
    _toSendEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(host), port);
    // if (mode == READ)
    //     bind(host, port);
}

babel::client::QtDataSocket::QtDataSocket(const Mode &mode)
    : _mode(mode), _ioService()
{
    _socket = std::make_unique<boost::asio::ip::udp::socket>(_ioService);
    _socket->open(boost::asio::ip::udp::v4());
    _socket->non_blocking(true);
}

babel::client::QtDataSocket::~QtDataSocket()
{
}

void babel::client::QtDataSocket::bind(const std::string &host, unsigned short port)
{
    std::cout << host << " : " << port << "\n";

    _toSendEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::make_address(host), port);
}

void babel::client::QtDataSocket::unbind()
{
    _socket->close();
}

void babel::client::QtDataSocket::sendData(const std::vector<uint16_t> &packet) const
{
    if (_toSendEndpoint.address().to_string() == "0.0.0.0"  || _toSendEndpoint.port() == 0) return;
    try
    {
        boost::asio::io_service ioService;
        boost::asio::ip::udp::resolver resolver(ioService);
        boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), _toSendEndpoint.address().to_string());

        std::cout << "Packet size: " << packet.size() << " to: " << _toSendEndpoint.address().to_string() << std::endl;

        // std::cout << "Writing to : " << _writeHost.toStdString() << " : " << _writePort << "\n";

        _socket->send_to(boost::asio::buffer(packet), *resolver.resolve(_toSendEndpoint), 0);
        std::cout << "Sent successfully.\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

std::vector<uint16_t> babel::client::QtDataSocket::getData()
{
    std::vector<uint16_t> packet;
    if (_toSendEndpoint.address().to_string() == "0.0.0.0" || _toSendEndpoint.port() == 0) return packet;
    try
    {
        std::array<char, 2210> buffer;
        
        boost::asio::ip::udp::endpoint sender;
        _socket->receive(boost::asio::buffer(buffer, 2210));
        void *data = buffer.data();
        packet = std::vector<uint16_t>(static_cast<uint16_t *>(data), (uint16_t*)data + buffer.size());
        std::cout << packet[0] << std::endl;
        // std::string res(buffer.begin(), buffer.end());
        // packet = convertStringToAudio(res);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return packet;
}