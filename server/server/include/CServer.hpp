/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** CServer
*/

#ifndef CSERVER_HPP_
#define CSERVER_HPP_

#include "CTcpServer.hpp"
#include "CFileManager.hpp"
#include "AProtocol.hpp"

#include <string>
#include <boost/asio.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/thread.hpp>

#ifndef BABEL_CONFIG_PATH
    #define BABEL_CONFIG_PATH "./files/server_config.json"
#endif

#include "IServer.hpp"

#include "JsonUtility.hpp"
#include "CUdpServer.hpp"

// #include "CCommandHandler.hpp"

namespace babel {
    namespace server {
        class CServer : public babel::server::IServer {
            public:
                CServer();
                ~CServer();

                int getPort() const { return _port; }
                std::string getDebugPath() const { return _debugPath; }

                bool popClientFromQueue();
                void readFromClients();
                void handleNewMessage(babel::server::CServerClient *);

                void init();
                void run();
                void loop();

                std::unique_ptr<babel::server::CFileManager> &getFileManager() { return _fileManager; }
                std::unique_ptr<babel::server::CTcpServer> &getTcpServer() { return _tcpServer; }
                // std::unique_ptr<babel::server::CCommandHandler> &getCommandHandler() { return _commandHandler; }

                IServer *getInstance() final
                {
                    return this;
                }

                void onClientConnection(babel::server::CServerClient &);
                void onClientDisconnect(babel::server::CServerClient &);

                void handleCommand(babel::server::CServerClient &, const babel::protocol::Message &);

                void requestConnection(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestCreateAccount(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestAddContact(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestRemoveContact(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestRequestCall(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestAcceptCall(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestHangCall(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestContactsInfo(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestOk(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestKo(babel::server::CServerClient &, const babel::protocol::Message &);

            // Udp servers' section
            private:
                void createUdpServer(const std::string &, int, const std::vector<babel::server::CUdpClient> &);
                void createUdpServer(const std::string &, int, const babel::server::CUdpClient &);
                void createUdpServer(const std::string &, int);
                void deleteUdpServer(const babel::server::CUdpServer &);
                void deleteUdpServer(int);
                void addClientToServer(std::shared_ptr<babel::server::CUdpServer> &, const babel::server::CUdpClient &);
                void addClientToServer(int , const babel::server::CUdpClient &);
                std::vector<std::shared_ptr<babel::server::CUdpServer> > _listUdpServer;
                std::vector<std::unique_ptr<boost::thread> > _listThreadUdp;

                unsigned short findAvailablePort() const noexcept
                {
                    unsigned short portAvailable = 0;
                    std::vector<unsigned short> copy(_portsAvailable.begin(), _portsAvailable.end());

                    for (const auto &port: _portsAvailable) {
                        for (const auto &server: _listUdpServer) {
                            if (port == server->getPort())
                                copy.erase(std::find(copy.begin(), copy.end(), port));
                        }
                    }
                    if (copy.size() > 0)
                        portAvailable = copy[0];
                    return portAvailable;
                };

                const std::array<unsigned short, 9> _portsAvailable = {
                    42423, 42424, 42425, 42427, 42428, 42429, 42430, 42431, 42432
                };
                
            private:
                std::unique_ptr<babel::server::CFileManager> _fileManager;
                std::unique_ptr<babel::server::CTcpServer> _tcpServer;
                // babel::server::CCommandHandler _commandHandler;
                std::vector<std::shared_ptr<babel::server::CServerClient> > _clients;
                unsigned short _port;
                std::string _debugPath;
        };
    }
}

#endif /* !CSERVER_HPP_ */
