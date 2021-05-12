/*
** EPITECH PROJECT, 2020
** ContactInfo
** File description:
** ContactInfo
*/

#include "ContactInfo.hpp"

babel::client::ContactInfo::ContactInfo(const std::string &username, unsigned int id)
    :_username(username)
    ,_id(id)
{
}

babel::client::ContactInfo::ContactInfo(const babel::client::ContactInfo &other)
{
    if (&other != this) {
        _username = other.getUsername();
        _id = other.getId();
    }
}

babel::client::ContactInfo::~ContactInfo()
{
}

bool babel::client::ContactInfo::operator==(const ContactInfo &other) const
{
    return (_username == other.getUsername() && _id == other.getId());
}