/*
** EPITECH PROJECT, 2020
** file_management
** File description:
** IFileManager
*/

#ifndef IFILEMANAGER_HPP_
#define IFILEMANAGER_HPP_

#include <string>
#include <vector>

namespace babel {
    namespace server {
        using QueryColumn = std::vector<std::pair<std::string, std::string> >;
        using QueryRows = std::vector<QueryColumn>;
    }
}

namespace babel {
    namespace server {
        class IFileManager {
            public:
                virtual ~IFileManager() {};

                virtual void OpenConnection(const std::string &) = 0;
                virtual void ExecuteCommand(const std::string &) = 0;
                virtual QueryRows GetResultCommand(const std::string &) = 0;
                virtual bool IsOpen() const = 0;
        };
    }
}

#endif /* !IFILEMANAGER_HPP_ */
