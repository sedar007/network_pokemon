#pragma once

namespace pokemon::tcp {

    class IConnection;

    template <typename T>
    class NETWORK_SOCKPP_POKEMON_API INetworkClientCommand {
    public:
        virtual ~INetworkClientCommand() = default;
        virtual void receive_from_server(T &context, std::shared_ptr<tcp_connector> connector) = 0;
    };
}


