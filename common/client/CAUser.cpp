/*
** EPITECH PROJECT, 2020
** client
** File description:
** CAUser
*/

#include "CAUser.hpp"

babel::common::CAUser::CAUser(const std::string &username, const std::string &password)
    : _username(username), _password(password)
{
}

babel::common::CAUser::CAUser()
    : _username(""), _password("")
{

}

babel::common::CAUser::~CAUser()
{
}

std::string babel::common::CAUser::getUsername() const
{
    return _username;
}

std::string babel::common::CAUser::getPassword() const
{
    return _password;
}

void babel::common::CAUser::operator=(const CAUser &other)
{
    if (&other != this) {
        _username = other.getUsername();
        _password = other.getPassword();
    }
}

void babel::common::CAUser::authentificate(const std::string &username, const std::string &password)
{
    setUsername(username);
    setPassword(password);
}

void babel::common::CAUser::setUsername(const std::string &username)
{
    _username = username;
}

void babel::common::CAUser::setPassword(const std::string &password)
{
    _password = password;
}
