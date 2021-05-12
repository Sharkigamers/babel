/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** JsonUtility
*/

#ifndef JSONUTILITY_HPP_
#define JSONUTILITY_HPP_

#include <map>
#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace babel {
    namespace common {
        class JsonUtility {
            public:
                JsonUtility();
                ~JsonUtility();

                static std::string mapToJson(const std::map<std::string, std::string> &);
                static std::map<std::string, std::string> jsonToMap(const std::string &);

            protected:
            private:
        };
    }
}

#endif /* !JSONUTILITY_HPP_ */
