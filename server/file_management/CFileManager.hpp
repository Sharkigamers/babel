/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** CFileManager
*/

#ifndef CFILEMANAGER_HPP_
#define CFILEMANAGER_HPP_

#include <memory>
#include <vector>
#include <iostream>
#include <boost/filesystem.hpp>

#include "IFileManager.hpp"
#include "CSQLite.hpp"
#include "CServerClient.hpp"

#define DEFAULT_DB_PATH "./files/database.db"

#define SELECT_USERS_QUERY "SELECT * FROM Users;"
#define CREATE_USERS_TABLE "CREATE TABLE Users (Id INTEGER PRIMARY KEY AUTOINCREMENT, Username TEXT NOT NULL, Password TEXT);"
#define ADD_USER_TABLE "INSERT INTO Users(Username, Password) values "
#define SELECT_EXIST_USER_QUERY(x, y) "SELECT Username FROM Users WHERE Username LIKE '" + x + "' AND Password LIKE '" + y + "'; "

#if defined(_WIN32)
#define SEPARATOR "\r\n"
#else
#define SEPARATOR "\n"
#endif

namespace babel {
    namespace server {
        class CFileManager {
            public:
                CFileManager(const std::string &, const std::string &);
                ~CFileManager();

                void createSqlInstance(const std::string &);

                std::vector<std::shared_ptr<babel::server::CServerClient> > getListUser() const;
                void addUser(const babel::server::CServerClient &);
                void addUser(const std::string &, const std::string &);
                void createUsersTable();
                bool userTableExist() const;

				std::string parseConfigFile() const;
                void setupDatabase();

                bool existUser(const babel::server::CServerClient &) const;
                bool existUser(const std::string &, const std::string &) const;

                bool authentificateUser(babel::server::CServerClient &, const std::string &, const std::string &) const;

            protected:
            private:
                std::unique_ptr<babel::server::IFileManager> _sqlInstance;
                std::string _configFilePath;
        };
    }
}

#endif /* !CFILEMANAGER_HPP_ */
