/*
** EPITECH PROJECT, 2020
** QtSocket.hpp
** File description:
** QtSocket
*/

#ifndef QTSOCKET_HPP_
#define QTSOCKET_HPP_

#include <stdint.h>
#include <string>
#include <QTcpSocket>
#include <QtNetwork>
#include <QtNetwork/qtcpsocket.h>
#include <QtNetwork/QTcpSocket>
#include <QtWidgets/QDialog>
#include <QDebug>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/core/ref.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <array>
#include <tuple>

#include "AProtocol.hpp"

namespace babel {
    namespace client {
        class QtSocket {
            public:
                QtSocket(const QString &, quint16);
                ~QtSocket();
                
                void readFromServer();
                template <typename T, typename Handler>
                void writeToServer(const T &t, Handler handler)
                {
                    std::ostringstream archive_stream;
                    boost::archive::text_oarchive archive(archive_stream);
                    archive << t;
                    std::string m_outbound_data = archive_stream.str();

                    std::ostringstream header_stream;
                    header_stream << std::setw(8)
                        << std::hex << m_outbound_data.size();
                    if (!header_stream || header_stream.str().size() != 8)
                    {
                        std::cout << "RIP" << std::endl;
                        return;
                    }
                    std::string m_outbound_header = header_stream.str();

                    std::vector<boost::asio::const_buffer> buffers;
                    buffers.push_back(boost::asio::buffer(m_outbound_header));
                    buffers.push_back(boost::asio::buffer(m_outbound_data));
                    std::string lopes = m_outbound_header + m_outbound_data;
                    _socket->write(lopes.c_str());
                }

            protected:
            private:
                std::unique_ptr<QTcpSocket> _socket;
        };
    }
}

#endif /* !QTSOCKET_HPP_ */
