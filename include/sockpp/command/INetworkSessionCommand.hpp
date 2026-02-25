#pragma once

namespace pokemon::tcp {

    class IConnection;

    template <typename T>
    class NETWORK_SOCKPP_POKEMON_API INetworkSessionCommand {
    public:
        virtual ~INetworkSessionCommand() = default;
        virtual void send_to_client(T &context, std::shared_ptr<IConnection> socket) = 0;
    };
}


