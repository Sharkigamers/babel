/*
** EPITECH PROJECT, 2020
** file_management
** File description:
** CSQLite
*/

#ifndef CSQLITE_HPP_
#define CSQLITE_HPP_

#include <memory>
#include <exception>
#include <sqlite3.h> 
#include <functional>

#include "IFileManager.hpp"

namespace babel {
    namespace server {
        class CSQLiteError : public std::exception {
            public:
                CSQLiteError(const std::string &);
                ~CSQLiteError();

                const char *what() const throw() { return _errorMessage.c_str(); }

            private:
                std::string _errorMessage;
        };
    }
}

namespace babel {
    namespace server {
        class CSQLite : public babel::server::IFileManager {
            public:
                CSQLite();
                ~CSQLite();
                
                void ExecuteCommand(const std::string &) final;
                babel::server::QueryRows GetResultCommand(const std::string &) final;
                void OpenConnection(const std::string &path) final;
                void ThrowSqliteError();
                void ThrowSqliteError(const std::string &);

                static int SqliteCallbackSelect(void *notUsed, int ac, char **av, char **res);
                bool IsOpen() const final { return _isOpen; }

            protected:
            private:
                void SetOpen(bool newState) { _isOpen = newState; }
                
                std::string _path;
                sqlite3 *_sqlInstance;
                int _error;
                std::string _errorMessage;
                bool _isOpen;
        };
    }
}

#endif /* !CSQLITE_HPP_ */
