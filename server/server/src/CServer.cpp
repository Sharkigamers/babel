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
        // bool isIpV4 = root.get<bool>("PREFER_IP_V4");
        _debugPath = root.get<std::string>("LOG.DEBUG_PATH");
        _fileManager = std::make_unique<CFileManager>(root.get<std::string>("SQL.DATABASE_PATH"), root.get<std::string>("SQL.CONFIG_PATH"));
        _tcpServer = std::make_unique<babel::server::CTcpServer>(true, _port, this);

        if (_fileManager->userTableExist() == false) {
            _fileManager->setupDatabase();
        } //else {
        //     _clients = _fileManager->getListUser();
        // }

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
        std::cout << "Oui" << std::endl;
        return true;
    }
    return false;
}

void babel::server::CServer::handleNewMessage(babel::server::CServerClient* client)
{
    std::shared_ptr<babel::protocol::Message> incomingMessage = client->getLastMessage();

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

void noop()
{}

void babel::server::CServer::onClientConnection(babel::server::CServerClient &client)
{
    const std::vector<int> contactList = _fileManager->getListContact(client.getId());

    for (const auto &id : contactList) {
        for (const auto &x: _tcpServer->getListClients()) {
            if (x->getId() == client.getId()) continue;
            if (x->getId() == id) {
                const std::map<std::string, std::string> infoFrom = {
                    {"ID", std::to_string(client.getId())},
                    {"Username", client.getUsername()},
                    {"Connected", "true"}
                };
                const std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createPartialContactsMessage(client.getId(), x->getId(), babel::common::JsonUtility::mapToJson(infoFrom));
                getTcpServer()->writeToClient(*x, *response, boost::bind(&noop));

                const std::map<std::string, std::string> infoTo = {
                    {"ID", std::to_string(x->getId())},
                    {"Username", x->getUsername()},
                    {"Connected", "true"}
                };

                const std::shared_ptr<babel::protocol::Message> responseTo = babel::protocol::AProtocol::createPartialContactsMessage(x->getId(), client.getId(), babel::common::JsonUtility::mapToJson(infoTo));
                getTcpServer()->writeToClient(client, *responseTo, boost::bind(&noop));

                break;
            }
        }
    }
}

void babel::server::CServer::onClientDisconnect(babel::server::CServerClient &client)
{
    const std::vector<int> contactList = _fileManager->getListContact(client.getId());

    for (const auto &id : contactList) {
        for (const auto &x: _tcpServer->getListClients()) {
            if (x->getId() == client.getId()) continue;
            if (x->getId() == id) {
                const std::map<std::string, std::string> infoFrom = {
                    {"ID", std::to_string(client.getId())},
                    {"Username", client.getUsername()},
                    {"Connected", "false"}
                };
                const std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createPartialContactsMessage(client.getId(), x->getId(), babel::common::JsonUtility::mapToJson(infoFrom));
                getTcpServer()->writeToClient(*x, *response, boost::bind(&noop));
                break;
            }
        }
    }
}

void babel::server::CServer::handleCommand(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    switch (message.getMessageType()) {
        case babel::protocol::REQUEST_CONNECTION:
            requestConnection(client, message);
            break;
        case babel::protocol::REQUEST_CREATE_ACCOUNT:
            requestCreateAccount(client, message);
            break;
        case babel::protocol::ADD_CONTACT:
            requestAddContact(client, message);
            break;
        case babel::protocol::REMOVE_CONTACT:
            requestRemoveContact(client, message);
            break;
        case babel::protocol::HANGUP:
            requestHangCall(client, message);
            break;
        case babel::protocol::REQUEST_CALL:
            requestRequestCall(client, message);
            break;
        case babel::protocol::ACCEPT_CALL:
            requestAcceptCall(client, message);
            break; 
        case babel::protocol::REQUEST_CONTACTS_INFO:
            requestContactsInfo(client, message);
            break;
        case babel::protocol::RESPONSE_OK:
            requestOk(client, message);
            break;
        case babel::protocol::RESPONSE_KO:
            requestKo(client, message);
            break;
        default:
            std::cerr << "Unknown command from client." << std::endl; 
            break;
    }
}

void babel::server::CServer::requestConnection(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    std::string username = objects["Username"];
    std::string password = objects["Password"];

    if (_fileManager->existUser(username, password) == false) {
        const std::map<std::string, std::string> errorMap = {{"Error", "User not found, please register"}};
        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseKO(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(errorMap));
        getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
    } else {
        _fileManager->authentificateUser(client, username, password);
        std::map<std::string, std::string> info = {
            {"ID", std::to_string(client.getId())}
        };
        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseOK(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(info));
        getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
        onClientConnection(client);
    }
}

void babel::server::CServer::requestCreateAccount(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    std::string username = objects["Username"];
    std::string password = objects["Password"];

    if (_fileManager->existUsername(username) == true) {
        const std::map<std::string, std::string> errorMap = {{"Error", "Username already taken."}};
        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseKO(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(errorMap));
        getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
    } else {
        client.authentificate(username, password);
        try
        {
            _fileManager->addUser(client);
            _fileManager->authentificateUser(client, username, password);
            std::map<std::string, std::string> info = {
                {"ID", std::to_string(client.getId())}
            };
            std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseOK(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(info));
            getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void babel::server::CServer::requestAddContact(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    const std::string usernameToFind = objects.at("Username");

    if (client.getUsername() == usernameToFind) return;

    if (_fileManager->existUser(usernameToFind, "")) {
        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseOK(SERVER_ID, client.getId(), babel::protocol::CALL, "");
        getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
        _fileManager->addContactList(client.getId(), usernameToFind);
    } else {
        const std::map<std::string, std::string> errorMap = {{"Error", "User not found."}};

        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseKO(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(errorMap));
        getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
    }
}

void babel::server::CServer::requestRemoveContact(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    const int idToDelete = std::atoi(objects.at("ID").c_str());

    _fileManager->removeContact(idToDelete);
    std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseOK(SERVER_ID, client.getId(), babel::protocol::CALL, "");
    getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
}

void babel::server::CServer::requestRequestCall(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    bool found = false;

    for (const auto &x : _tcpServer->getListClients()) {
        if (x->getId() == message.toId()) {
            found = true;

            unsigned short portToUse = findAvailablePort();
            createUdpServer("127.0.0.1", portToUse, babel::server::CUdpClient(client.getTcpConnection()->getSocket()->remote_endpoint().address().to_string(), portToUse));

            const std::map<std::string, std::string> info = {
                {"Port", std::to_string(portToUse)},
                {"ServerID", std::to_string(_listUdpServer[_listUdpServer.size() -1]->getId())}
            };
            std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createCallMessage(client.getId(), x->getId(), babel::common::JsonUtility::mapToJson(info));
            std::shared_ptr<babel::protocol::Message> responseOk = babel::protocol::AProtocol::createResponseOK(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(info));
            getTcpServer()->writeToClient(*x, *response, boost::bind(&noop));
            getTcpServer()->writeToClient(client, *responseOk, boost::bind(&noop));
        }
    }
    if (!found) {
        const std::map<std::string, std::string> errorMap = {{"Error", "User not connected."}};

        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseKO(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(errorMap));
        getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
    }
}

void babel::server::CServer::requestAcceptCall(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    // const std::string host = objects.at("Host");
    const unsigned short port = static_cast<unsigned short>(std::stoi(objects.at("Port")));
    bool found = false;

    std::cout << "Accept from :" << client.getUsername() << std::endl;

    for (const auto &x : _tcpServer->getListClients()) {
        if (x->getId() == message.toId()) {
            found = true;
            const int serverId = std::stoi(objects.at("ServerID"));
            const std::map<std::string, std::string> info = {
                {"Port", std::to_string(port)},
                {"ServerID", std::to_string(serverId)}
            };
            addClientToServer(serverId, babel::server::CUdpClient(client.getTcpConnection()->getSocket()->remote_endpoint().address().to_string(), _listUdpServer[_listUdpServer.size() -1]->getPort()));
            std::shared_ptr<babel::protocol::Message> responseOk = babel::protocol::AProtocol::createAcceptCallMessage(client.getId(), x->getId(), babel::common::JsonUtility::mapToJson(info));
            // std::shared_ptr<babel::protocol::Message> responseOkToClient = babel::protocol::AProtocol::createResponseOK(x->getId(), client.getId(), babel::protocol::messageCategory::CALL, babel::common::JsonUtility::mapToJson(info));
            getTcpServer()->writeToClient(*x, *responseOk, boost::bind(&noop));
            // getTcpServer()->writeToClient(client, *responseOkToClient, boost::bind(&noop));
        }
    }
    if (!found) {
        const std::map<std::string, std::string> errorMap = {{"Error", "User not connected."}};

        std::shared_ptr<babel::protocol::Message> response = babel::protocol::AProtocol::createResponseKO(SERVER_ID, client.getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(errorMap));
        getTcpServer()->writeToClient(client, *response, boost::bind(&noop));
    }
}

void babel::server::CServer::requestHangCall(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    // const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    (void)client;

    for (const auto &x : _tcpServer->getListClients()) {
        if (x->getId() == message.toId()) {
            _listUdpServer[0]->stop();
            std::shared_ptr<babel::protocol::Message> responseHangUp = babel::protocol::AProtocol::createHangupMessage(client.getId(), x->getId(), message.getMessage());
            getTcpServer()->writeToClient(*x, *responseHangUp, boost::bind(&noop));
        }
    }
}

void babel::server::CServer::requestContactsInfo(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    (void)client;   
}

void babel::server::CServer::requestOk(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());
    (void)client;

    switch (message.getMessageCategory()) {
        case babel::protocol::CALL:
            break;
        default:
            break;
    }    
}

void babel::server::CServer::requestKo(babel::server::CServerClient &client, const babel::protocol::Message &message)
{
    const std::map<std::string, std::string> objects = babel::common::JsonUtility::jsonToMap(message.getMessage());

    if (message.getMessageCategory() == babel::protocol::CALL) {
        for (const auto &x : _tcpServer->getListClients()) {
            if (x->getId() == message.toId()) {
                const std::map<std::string, std::string> errorMap = {{"Error", "Connection refused."}};

                std::shared_ptr<babel::protocol::Message> responseKo = babel::protocol::AProtocol::createResponseKO(client.getId(), x->getId(), babel::protocol::CALL, babel::common::JsonUtility::mapToJson(errorMap));
                getTcpServer()->writeToClient(*x, *responseKo, boost::bind(&noop));
            }
        }
    }
}


void babel::server::CServer::createUdpServer(const std::string &host, int port, const std::vector<babel::server::CUdpClient> &clients)
{
    std::shared_ptr<babel::server::CUdpServer> server = std::make_shared<babel::server::CUdpServer>(host, port, std::rand(), clients);

    _listUdpServer.push_back(server);
}

void babel::server::CServer::createUdpServer(const std::string &host, int port, const babel::server::CUdpClient &client)
{
    std::shared_ptr<babel::server::CUdpServer> server = std::make_shared<babel::server::CUdpServer>(host, port, std::rand(), client);
    _listUdpServer.push_back(server);
    _listThreadUdp.push_back(std::make_unique<boost::thread>([this]() {
        // _listUdpServer[_listUdpServer.size() - 1]->startReceive();
        boost::thread read = boost::thread([this]() {
            _listUdpServer[_listUdpServer.size() - 1]->startReceive();
        });
        boost::thread write = boost::thread([this]() {
            _listUdpServer[_listUdpServer.size() - 1]->startWrite();
        });
        read.join();
        write.join();
    }));
}

void babel::server::CServer::createUdpServer(const std::string &host, int port)
{
    _listUdpServer.push_back(std::make_shared<babel::server::CUdpServer>(host, port, std::rand()));
}

void babel::server::CServer::addClientToServer(std::shared_ptr<babel::server::CUdpServer> &server, const babel::server::CUdpClient &client)
{
    server->addClient(client);
}

void babel::server::CServer::addClientToServer(int idServer, const babel::server::CUdpClient &client)
{
    try
    {
        for (auto &x : _listUdpServer) {
            if (idServer == x->getId())
                x->addClient(client);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Server n°" << idServer << " does not exist." << "\n";
    }
    
}

void babel::server::CServer::deleteUdpServer(const babel::server::CUdpServer &server)
{
    try
    {
        // _listUdpServer.erase(std::find(_listUdpServer.begin(), _listUdpServer.end(), server));
    }
    catch(const std::exception &e)
    {
        std::cerr << "Server n°" << server.getId() << " does not exist." << std::endl;
    }
}

void babel::server::CServer::deleteUdpServer(int idServer)
{
    try
    {
        // _listUdpServer.erase(std::find(_listUdpServer.begin(), _listUdpServer.end(), idServer));
    }
    catch(const std::exception &e)
    {
        std::cerr << "Server n°" << idServer << " does not exist." << std::endl;
    }
}