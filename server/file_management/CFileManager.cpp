/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** CFileManager
*/

#include "CFileManager.hpp"

#include <filesystem>

babel::server::CFileManager::CFileManager(const std::string &path, const std::string &configFilePath)
    : _configFilePath(configFilePath)
{
    createSqlInstance(path);
}

babel::server::CFileManager::~CFileManager()
{
}

void babel::server::CFileManager::createSqlInstance(const std::string &path)
{
    // On créer une instance de IFileManager, içi avec SQLite3
    _sqlInstance = std::make_unique<CSQLite>();
    _sqlInstance->OpenConnection(path);
}

std::vector<std::shared_ptr<babel::server::CServerClient> > babel::server::CFileManager::getListUser() const
{
    std::vector<std::shared_ptr<babel::server::CServerClient> > res;
    babel::server::QueryRows queryResult;

    try
    {
        queryResult = _sqlInstance->GetResultCommand(SELECT_USERS_QUERY);
        for (const auto &x : queryResult) {
            int id = std::stoi(x[0].second);
            std::string username = x[1].second;
            std::string password = x[2].second;
            std::shared_ptr<babel::server::CServerClient> client = std::make_shared<babel::server::CServerClient>(id, username, password, false);
            res.push_back(client);
        }
    }
    catch(const std::exception& e)
    {
        throw e;
    }
    return res;
}

void babel::server::CFileManager::createUsersTable()
{
    try
    {
        if (_sqlInstance->IsOpen()) {
            _sqlInstance->ExecuteCommand(CREATE_USERS_TABLE);
        }
    }
    catch(const std::exception& e)
    {
        throw e;
    }
}

void babel::server::CFileManager::addUser(const babel::server::CServerClient &client)
{
    try
    {
        if (_sqlInstance->IsOpen()) {
            std::string addClientQuery = ADD_USER_TABLE + client.toSQLString();
            _sqlInstance->ExecuteCommand(addClientQuery);
        }
    }
    catch(const std::exception& e)
    {
        throw e;
    }
    
}

void babel::server::CFileManager::addUser(const std::string &username, const std::string &password)
{
    try
    {
        if (_sqlInstance->IsOpen()) {
            std::string addClientQuery = std::string(ADD_USER_TABLE) + "('" + username + "','" + password + "');";
            _sqlInstance->ExecuteCommand(addClientQuery);
        }
    }
    catch(const std::exception& e)
    {
        throw e;
    }
    
}

bool babel::server::CFileManager::userTableExist() const
{
    const std::string query = "SELECT name FROM sqlite_master WHERE name LIKE 'Users';";
    babel::server::QueryRows queryResult;

    if (_sqlInstance->IsOpen()) {
        queryResult = _sqlInstance->GetResultCommand(query);

        if (queryResult.empty() == true) {
            return false;
        } else 
            return true;
    }
    return false;
}

std::string babel::server::CFileManager::parseConfigFile() const
{
	std::ifstream ifs(_configFilePath);
	std::string queryFile;
	std::string tmp;

	while (std::getline(ifs, tmp)) {
		queryFile += tmp + SEPARATOR;
	}
	return queryFile;
}

void babel::server::CFileManager::setupDatabase()
{
	std::string queryFile = parseConfigFile();

    if (_sqlInstance->IsOpen()) {
        try
        {
            _sqlInstance->ExecuteCommand(queryFile);
        }
        catch(const std::exception& e)
        {
            throw e;
        }
    }
}

bool babel::server::CFileManager::existUser(const babel::server::CServerClient &client) const
{
    std::string query = SELECT_EXIST_USER_QUERY(client.getUsername(), client.getPassword());
    babel::server::QueryRows queryResult;

    if (_sqlInstance->IsOpen()) {
        try
        {
            queryResult = _sqlInstance->GetResultCommand(query);

            if (queryResult.empty() == true) {
                return false;
            } else 
                return queryResult[0][0].second == client.getUsername();
        }
        catch(const std::exception& e)
        {
            throw e;
        }
    }
    return false;
}

bool babel::server::CFileManager::existUser(const std::string &username, const std::string &password) const
{
    const std::string query = SELECT_EXIST_USER_QUERY(username, password);
    babel::server::QueryRows queryResult;

    if (_sqlInstance->IsOpen()) {
        try
        {
            queryResult = _sqlInstance->GetResultCommand(query);

            if (queryResult.empty() == true) {
                return false;
            } else 
                return queryResult[0][0].second == username;        
        }
        catch(const std::exception& e)
        {
            throw e;
        }
    }
    return false;
}

bool babel::server::CFileManager::authentificateUser(babel::server::CServerClient &client, const std::string &username, const std::string &password) const
{
    const std::string statement = "SELECT * FROM Users WHERE Username = " + username + " AND Password = " + password + ";";

    if (existUser(username, password) == false) {
        return false;
    } else {
        if (_sqlInstance->IsOpen()) {
            try
            {
                babel::server::QueryRows queryResult = _sqlInstance->GetResultCommand(statement);

                if (queryResult.empty() == true) {
                    return false;
                } else {
                    const int id = std::stoi(queryResult[0][0].second);
                    const std::string username = queryResult[0][1].second;
                    const std::string password = queryResult[0][2].second;

                    client.authentificateClient(id, username, password);
                }
            }
            catch(const std::exception& e)
            {
                throw e;
            }
        }
    }
    return true;
}
