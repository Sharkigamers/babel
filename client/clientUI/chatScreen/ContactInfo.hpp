/*
** EPITECH PROJECT, 2020
** ContactInfo
** File description:
** ContactInfo
*/

#ifndef CONTACTINFO_HPP_
#define CONTACTINFO_HPP_
#include <string>

namespace babel {
    namespace client {
        class ContactInfo {
            public:
                ContactInfo(const std::string &, unsigned int);
                ContactInfo(const ContactInfo &other);
                ~ContactInfo();

                bool operator==(const ContactInfo &) const;

                std::string getUsername() const {
                    return (_username);
                }
                unsigned int getId() const {
                    return (_id);
                }
                void serUsername(std::string &username) {
                    _username = username;
                }
                void setId(unsigned int id) {
                    _id = id;
                }

            protected:
            private:
                std::string _username;
                unsigned int _id;
        };
    }
}

#endif /* !CONTACTINFO_HPP_ */
