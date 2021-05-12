/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** CServer
*/

#include "CServer.hpp"
#include <iostream>

babel::server::CServer::CServer()
{
}

babel::server::CServer::~CServer()
{
}

void babel::server::CServer::init()
{
    boost::property_tree::ptree root;

    try
    {
        boost::property_tree::read_json(BABEL_CONFIG_PATH, root);
        _port = root.get<unsigned short>("PORT");
        bool isIpV4 = root.get<bool>("PREFER_IP_V4");
        _debugPath = root.get<std::string>("LOG.DEBUG_PATH");
        _fileManager = std::make_unique<CFileManager>(root.get<std::string>("SQL.DATABASE_PATH"), root.get<std::string>("SQL.CONFIG_PATH"));
        _tcpServer = std::make_unique<babel::server::CTcpServer>(true, _port, this);

        if (_fileManager->userTableExist() == false) {
            _fileManager->setupDatabase();
        } else {
            _clients = _fileManager->getListUser();
        }

        // std::cout << "Clients : " <<_clients.size() << std::endl;;
    }
    catch(const std::exception& e)
    {
        throw e;
    }
}

bool babel::server::CServer::popClientFromQueue()
{
    if (_tcpServer->isNewClientAvailable() == true) {
        std::shared_ptr<babel::server::CServerClient> lastClient = _tcpServer->getLastClient();
        _clients.push_back(lastClient);
        return true;
    }
    return false;
}

void babel::server::CServer::handleNewMessage(babel::server::CServerClient* client)
{
    std::shared_ptr<babel::protocol::Message> incomingMessage = client->getLastMessage();

    std::cout << incomingMessage->getMessage() << std::endl;
    std::cout << (incomingMessage->getMessageType() == babel::protocol::REQUEST_CONNECTION ? "Lopes" : "Pas lopes") << std::endl;
    handleCommand(*client, *incomingMessage);
}

void babel::server::CServer::readFromClients()
{
    // boost::mutex mutex;

    // mutex.lock();
    // for (std::size_t i = 0; i < _clients.size(); i += 1) {
    //     if (_clients[i]->isLogedIn() == false) {
    //         _clients.erase(_clients.begin() + i);
    //         break;
    //     }
    //     if (_clients[i]->isLogedIn() == true && _clients[i]->isNewMessage() == true) {
    //         handleNewMessage(_clients[i]);
    //     }
    // }
    // mutex.unlock();
}

void babel::server::CServer::run()
{
    _tcpServer->waitClient();
    _tcpServer->getIoService().run();
}

void babel::server::CServer::loop()
{
    while (1) {
        
    }
}

void babel::server::CServer::handleCommand(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    switch (message.getMessageType()) {
        case babel::protocol::REQUEST_CONNECTION:
            requestConnection(client, message);
            break;
        default:
            break;
    }
}

void issou2()
{

}

void babel::server::CServer::requestConnection(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    std::string username = objects["Username"];
    std::string password = objects["Password"];

    if (_fileManager->existUser(username, password) == false) {
        const std::map<std::string, std::string> errorMap = {{"Error", "User not found, please register"}};
        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseKO(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(errorMap));
        getTcpServer()->writeToClient(client, *response, boost::bind(&issou2));
    } else {
        _fileManager->authentificateUser(client, username, password);
        std::map<std::string, std::string> info = {
            {"ID", std::to_string(client.getId())}
        };
        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseOK(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(info));
        getTcpServer()->writeToClient(client, *response, boost::bind(&issou2));
    }
}

void babel::server::CServer::requestCreateAccount(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    
}

void babel::server::CServer::requestAddContact(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    
}

void babel::server::CServer::requestRemoveContact(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    
}

void babel::server::CServer::requestRequestCall(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    
}

void babel::server::CServer::requestHangCall(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    
}

void babel::server::CServer::requestContactsInfo(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    
}

