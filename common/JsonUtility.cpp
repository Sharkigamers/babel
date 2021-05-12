/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** JsonUtility
*/

#include "JsonUtility.hpp"
#include <iostream>
babel::common::JsonUtility::JsonUtility()
{
}

babel::common::JsonUtility::~JsonUtility()
{
}

std::string babel::common::JsonUtility::mapToJson(const std::map<std::string, std::string> &map)
{
    boost::property_tree::ptree pTree;
    std::ostringstream buffer;

    for (const auto &x : map)
        pTree.put(x.first, x.second);
    boost::property_tree::write_json(buffer, pTree, false);
    return buffer.str();
}

std::map<std::string, std::string> babel::common::JsonUtility::jsonToMap(const std::string &string)
{
    std::map<std::string, std::string> map;
    boost::property_tree::ptree pTree;
    std::istringstream buffer(string);

    boost::property_tree::read_json(buffer, pTree);
    for (boost::property_tree::ptree::const_iterator it = pTree.begin(); it != pTree.end(); it++) {
        map[it->first] = it->second.get_value<std::string>();
    }
    return map;
}