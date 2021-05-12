/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** IProtocol
*/

#ifndef IPROTOCOL_HPP_
#define IPROTOCOL_HPP_

#define MESSAGE_LENGTH 2048

#include <algorithm>
#include <string>
#include <string.h>

#include <fstream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace babel {
    namespace protocol {

#define NOT_CONNECTED_ID -2
#define SERVER_ID -1

        enum messageType {
            NONE,
            RESPONSE_OK,
            RESPONSE_KO,
            REQUEST_CALL,
            ACCEPT_CALL,
            HANGUP,
            ADD_CONTACT,
            REMOVE_CONTACT,
            CONTACT_INFO,
            MUTE,
            REQUEST_CONTACTS_INFO,
            REQUEST_CONNECTION,
            REQUEST_CREATE_ACCOUNT
        };

        enum messageCategory {
            NO_CATEGORY,
            CONNECTION,
            CALL,
            CONTACT
        };

        enum messageStatus {
            NO_STATUS,
            COMPLETED,
            INCOMPLETE
        };

        class Message {
            public:
                Message(bool isRequest, int fromId, int toId, babel::protocol::messageType messageType, babel::protocol::messageCategory messageCategory, babel::protocol::messageStatus messageStatus, const std::string &messageToCopy)
                    : _isRequest(isRequest), _fromId(fromId), _toId(toId), _messageType(messageType), _messageCategory(messageCategory),_messageStatus(messageStatus), _message(messageToCopy)
                {}

                Message(const Message &other)
                    : _isRequest(other.isRequest()), _fromId(other.fromId()), _toId(other.toId()), _messageType(other.getMessageType()), _messageCategory(other.getMessageCategory()),_messageStatus(other.getMessageStatus()), _message(other.getMessage())
                {}

                Message &operator=(const Message &other)
                {
                    if (&other != this) {
                        _isRequest = other.isRequest();
                        _fromId = other.fromId();
                        _toId = other.toId();
                        _messageType = other.getMessageType();
                        _messageCategory = other.getMessageCategory();
                        _messageStatus = other.getMessageStatus();
                        _message = other.getMessage();
                    }
                    return *this;
                }

                Message()
                    : _isRequest(false), _fromId(0), _toId(0), _messageType(NONE), _messageCategory(NO_CATEGORY), _messageStatus(NO_STATUS), _message("")
                {};
                ~Message() {};

                int size() const noexcept
                {
                    return sizeof(bool) + sizeof(int) * 2 + sizeof(babel::protocol::messageType) + sizeof(babel::protocol::messageCategory) + sizeof(babel::protocol::messageStatus) + _message.length();
                }

                bool isRequest() const
                {
                    return _isRequest;
                }

                int fromId() const
                {
                    return _fromId;
                }

                int toId() const
                {
                    return _toId;
                }

                babel::protocol::messageType getMessageType() const
                {
                    return _messageType;
                }

                babel::protocol::messageCategory getMessageCategory() const
                {
                    return _messageCategory;
                }

                babel::protocol::messageStatus getMessageStatus() const
                {
                    return _messageStatus;
                }

                std::string getMessage() const
                {
                    return _message;
                }

                friend class boost::serialization::access;
                template<class Archive>
                void serialize(Archive& ar, const unsigned int version)
                {
                    (void)version;
                    ar & _isRequest & _fromId & _toId & _messageType & _messageCategory & _messageStatus & _message;
                }

            private:
                bool _isRequest;
                int _fromId;
                int _toId;
                babel::protocol::messageType _messageType;
                babel::protocol::messageCategory _messageCategory;
                babel::protocol::messageStatus _messageStatus;
                std::string _message;
        };

    }
};

#endif /* !IPROTOCOL_HPP_ */
