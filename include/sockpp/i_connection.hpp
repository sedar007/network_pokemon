#pragma once

namespace pokemon::tcp {

    class NETWORK_SOCKPP_POKEMON_API IConnection {
        public:
            virtual ~IConnection() = default;
            virtual bool is_open() const = 0;
            virtual std::string address() const = 0;
            virtual ssize_t read(void* buf, size_t n) = 0;
            virtual ssize_t write(const void* buf, size_t n) = 0;
    };
}


