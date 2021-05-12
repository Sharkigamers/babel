/*
** EPITECH PROJECT, 2020
** Babel
** File description:
** IOpus
*/

#ifndef IOPUS_HPP_
#define IOPUS_HPP_

#include <vector>

namespace babel {
    namespace client {
        class IOpus {
            public:
                IOpus() {};

                virtual void create_encoder() = 0;
                virtual void create_decoder() = 0;

                virtual void destroy_encoder() = 0;
                virtual void destroy_decoder() = 0;

                virtual std::vector<uint16_t> encode(const std::vector<uint16_t> &encodableBuffer) = 0;
                virtual std::vector<uint16_t> decode(const std::vector<uint16_t> &encoded) = 0;

            protected:
            private:
        };
    }
}

#endif /* !IOPUS_HPP_ */
