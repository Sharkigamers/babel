/*
** EPITECH PROJECT, 2020
** file_management
** File description:
** CSQLite
*/

#include "CSQLite.hpp"

#include <iostream>
#include <algorithm>

babel::server::CSQLite::CSQLite()
{
    _errorMessage = "";
    SetOpen(false);
}

babel::server::CSQLite::~CSQLite()
{
    sqlite3_close(_sqlInstance);
}

int babel::server::CSQLite::SqliteCallbackSelect(void *input, int ac, char **av, char **res)
{
    babel::server::QueryRows *result = static_cast<babel::server::QueryRows *>(input);
    int nbCols = 0, i = 0;

    for (; av[nbCols] != nullptr; nbCols++);
    for (; i < ac;) {
        babel::server::QueryColumn newColumn;
        for (int j = 0; j < nbCols; j++, i++) {
            newColumn.push_back(std::make_pair(res[j], av[i] ? av[i] : std::string("NULL")));
        }
        result->push_back(newColumn);
    }
    return 0;
}

int SqliteCallback(void *notUsed, int ac, char **av, char **res)
{
    (void)notUsed;
    (void)ac;
    (void)av;
    (void)res;
    return 0;
}

void babel::server::CSQLite::OpenConnection(const std::string &path)
{
    if (path.length() < 4) {
        ThrowSqliteError("Invalid name");
    }
    _error = sqlite3_open(path.data(), &_sqlInstance);
    if (_error != SQLITE_OK) {
        ThrowSqliteError();
    }
    SetOpen(true);
}

void babel::server::CSQLite::ExecuteCommand(const std::string &command)
{
    char *error = _errorMessage.data();

    _error = SQLITE_OK;
    if (IsOpen() == false) {
        ThrowSqliteError();
    }

    _error = sqlite3_exec(_sqlInstance, command.data(), SqliteCallback, 0, &error);

    if (_error != SQLITE_OK) {
        ThrowSqliteError(error);
    }
}

babel::server::QueryRows babel::server::CSQLite::GetResultCommand(const std::string &command)
{
    char *error = _errorMessage.data();
    babel::server::QueryRows result;

    _error = SQLITE_OK;
    if (IsOpen() == false) {
        ThrowSqliteError();
    }

    _error = sqlite3_exec(_sqlInstance, command.data(), SqliteCallbackSelect, &result, &error);

    if (_error != SQLITE_OK) {
        ThrowSqliteError(error);
    }
    return result;
}


void babel::server::CSQLite::ThrowSqliteError()
{
    if (_error != SQLITE_OK)
        throw babel::server::CSQLiteError(std::string(sqlite3_errmsg(_sqlInstance)));
}

void babel::server::CSQLite::ThrowSqliteError(const std::string &error)
{
    throw babel::server::CSQLiteError(error);
}

/* 
    Exception
*/

babel::server::CSQLiteError::CSQLiteError(const std::string &message)
    : _errorMessage(message)
{
}

babel::server::CSQLiteError::~CSQLiteError()
{

}