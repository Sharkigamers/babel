/*
** EPITECH PROJECT, 2020
** B-CPP-500-PAR-5-1-babel-emmanuel.blineau
** File description:
** IServer
*/

#ifndef ISERVER_HPP_
#define ISERVER_HPP_

namespace babel {
    namespace server {
        class IServer {
            public:
                virtual ~IServer() = default;

                virtual IServer *getInstance() = 0;

            protected:
            private:
        };
    }
}

#endif /* !ISERVER_HPP_ */
