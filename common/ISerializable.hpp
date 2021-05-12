/*
** EPITECH PROJECT, 2020
** OOP_babel_2019
** File description:
** ISerializable
*/

#ifndef ISERIALIZABLE_HPP_
#define ISERIALIZABLE_HPP_

#include <string>

namespace babel {
    namespace common {
        class ISQLSerializable {
            public:
                virtual ~ISQLSerializable() {};

                virtual std::string toSQLString() const = 0;

            protected:
            private:
        };
    }
}

#endif /* !ISERIALIZABLE_HPP_ */
