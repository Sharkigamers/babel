/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** AProtocol
*/

#ifndef APROTOCOL_HPP_
#define APROTOCOL_HPP_

#include "IProtocol.hpp"

#include <string>
#include <memory>

namespace babel {
    namespace protocol {
        class AProtocol {
            public:
                AProtocol();
                ~AProtocol();

                static std::shared_ptr<babel::protocol::Message> createMessage(bool, int, int, babel::protocol::messageType, babel::protocol::messageCategory, babel::protocol::messageStatus, const std::string &message);
                static std::shared_ptr<babel::protocol::Message> createConnectionMessage(int, int, const std::string &message);
                static std::shared_ptr<babel::protocol::Message> createCallMessage(int, int, const std::string &message);
                static std::shared_ptr<babel::protocol::Message> createHangupMessage(int, int, const std::string &message);
                static std::shared_ptr<babel::protocol::Message> createAddContactMessage(int, int, const std::string &message);
                static std::shared_ptr<babel::protocol::Message> createGetAllContactsMessage(int, int, const std::string &message);
                static std::shared_ptr<babel::protocol::Message> createMuteContactMessage(int, int, const std::string &message);
                
                static std::shared_ptr<babel::protocol::Message> createResponseOK(int, int, babel::protocol::messageCategory, const std::string &message);
                static std::shared_ptr<babel::protocol::Message> createResponseKO(int, int, babel::protocol::messageCategory, const std::string &message);

            protected:
            private:
        };

    }
}

#endif /* !APROTOCOL_HPP_ */
