#pragma once

namespace pokemon::tcp {

    class IConnection;

    template <typename T>
    class NETWORK_SOCKPP_POKEMON_API ISessionCommand {
    public:
        virtual ~ISessionCommand() = default;
        virtual void send_to_client_test(T &context, std::shared_ptr<IConnection> socket) = 0;
    };
}


