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

                void handleCommand(babel::server::CServerClient &, const babel::protocol::Message &);

                void requestConnection(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestCreateAccount(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestAddContact(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestRemoveContact(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestRequestCall(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestHangCall(babel::server::CServerClient &, const babel::protocol::Message &);
                void requestContactsInfo(babel::server::CServerClient &, const babel::protocol::Message &);

            protected:
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
