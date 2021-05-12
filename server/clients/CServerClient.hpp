/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** CServerClient
*/

#ifndef CSERVERCLIENT_HPP_
#define CSERVERCLIENT_HPP_

#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include <array>

#include "common/client/CAUser.hpp"
#include "common/ISerializable.hpp"
#include "common/protocol/IProtocol.hpp"

#include "CTcpConnection.hpp"

#include "IServer.hpp"

namespace babel {
    namespace server {
        class CServerClient : public std::enable_shared_from_this<CServerClient>,  public babel::common::CAUser, public babel::common::ISQLSerializable {
            public:
                explicit CServerClient(int id, const std::string &username, const std::string &password, bool isConnected);
                explicit CServerClient(std::shared_ptr<babel::server::CTcpConnection> &tcpConnection);
                explicit CServerClient(const CServerClient &);
                explicit CServerClient();
                ~CServerClient();

                CServerClient &operator=(const CServerClient &other);
                std::string toSQLString() const final;

                void setTcpConnection(std::shared_ptr<babel::server::CTcpConnection> tcpConnection) { _tcpConnection = tcpConnection; };
                std::shared_ptr<babel::server::CTcpConnection> getTcpConnection() const { return _tcpConnection; };

                bool isNewMessage() const { return !_messageQueue.empty(); }
                std::shared_ptr<babel::protocol::Message> getLastMessage();

                std::string getClientInfo() const;
                int getId() const { return _id; }
                void setId(int id) { _id = id; }
                
                bool isConnected() const { return _isConnected; }
                
                bool isLogedIn() const { return _isLogedIn; }
                void setIsLogedIn(bool state) { _isLogedIn = state; }

                void authentificateClient(int id, const std::string &username, const std::string &password);

                void readFromClient();
                std::vector<std::shared_ptr<babel::protocol::Message> > &getMessageQueue() { return _messageQueue; }
                void setServer(babel::server::IServer *server) { _server = server; }
            protected:
            private:
                int _id;
                bool _isConnected;
                bool _isLogedIn = false;
                std::shared_ptr<babel::server::CTcpConnection> _tcpConnection;
                std::vector<std::shared_ptr<babel::protocol::Message> > _messageQueue;
                babel::server::IServer *_server;
                char _readBuffer[4096];
        };
    }
}

#endif /* !CSERVERCLIENT_HPP_ */
