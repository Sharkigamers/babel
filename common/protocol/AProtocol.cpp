/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** AProtocol
*/

#include "AProtocol.hpp"

babel::protocol::AProtocol::AProtocol()
{
}

babel::protocol::AProtocol::~AProtocol()
{
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createMessage(bool isRequest, int fromId, int toId, babel::protocol::messageType type, babel::protocol::messageCategory category, babel::protocol::messageStatus status, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = std::make_shared<babel::protocol::Message>(isRequest, fromId, toId, type, category, status, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createConnectionMessage(int fromId, int toId, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, REQUEST_CONNECTION, CONNECTION, NO_STATUS, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createCallMessage(int fromId, int toId, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, REQUEST_CALL, CALL, NO_STATUS, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createHangupMessage(int fromId, int toId, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, HANGUP, CALL, NO_STATUS, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createAddContactMessage(int fromId, int toId, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, ADD_CONTACT, CONTACT, NO_STATUS, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createGetAllContactsMessage(int fromId, int toId, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, REQUEST_CONTACTS_INFO, CONTACT, NO_STATUS, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createMuteContactMessage(int fromId, int toId, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, MUTE, CALL, NO_STATUS, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createResponseOK(int fromId, int toId, babel::protocol::messageCategory category, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, RESPONSE_OK, category, NO_STATUS, message);

    return newMessage;
}

std::shared_ptr<babel::protocol::Message> babel::protocol::AProtocol::createResponseKO(int fromId, int toId, babel::protocol::messageCategory category, const std::string &message)
{
    std::shared_ptr<babel::protocol::Message> newMessage = createMessage(true, fromId, toId, RESPONSE_KO, category, NO_STATUS, message);

    return newMessage;
}
