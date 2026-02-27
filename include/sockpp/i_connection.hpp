#pragma once

namespace pokemon::tcp {

    class NETWORK_SOCKPP_POKEMON_API IConnection {
        public:
            virtual ~IConnection() = default;
            virtual bool is_open() const = 0;
            virtual std::string address() const = 0;
            virtual bool read( std::byte* buffer, size_t length) = 0;
            virtual ssize_t write(const void* buf, size_t n) = 0;
            virtual void shutdown() = 0;
            virtual std::string address_to_string() const = 0;
    };
}


