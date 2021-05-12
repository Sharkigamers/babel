/*
** EPITECH PROJECT, 2020
** client
** File description:
** CAUser
*/

#ifndef CAUSER_HPP_
#define CAUSER_HPP_

#include <string>

namespace babel {
    namespace common {
        class CAUser {
            public:
                CAUser(const std::string &username, const std::string &password);
                CAUser();
                ~CAUser();

                std::string getUsername() const;
                std::string getPassword() const;
                
                void operator=(const CAUser &);

                void authentificate(const std::string &, const std::string &);
            protected:
            private:
                void setUsername(const std::string &);
                void setPassword(const std::string &);

                std::string _username;
                std::string _password;
        };
    }
}

#endif /* !CAUSER_HPP_ */
