/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** CUdpClient
*/

#ifndef CUDPCLIENT_HPP_
#define CUDPCLIENT_HPP_

#include <string>
#include <boost/asio.hpp>

namespace babel {
    namespace server {
        class CUdpClient {
            public:
                CUdpClient(const std::string &addr, unsigned short);
                ~CUdpClient();

                boost::asio::ip::udp::endpoint getEndpoint() const { return _clientEndpoint; }

            protected:
            private:
                boost::asio::ip::udp::endpoint _clientEndpoint;
        };
    }
}



#endif /* !CUDPCLIENT_HPP_ */
