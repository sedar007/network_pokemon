#pragma once

namespace pokemon::tcp {

    class IConnection;

    template <typename T>
    class NETWORK_SOCKPP_POKEMON_API INetworkCommand {
    public:
        virtual ~INetworkCommand() = default;
        virtual void send_to_client(T &context, std::shared_ptr<tcp::IConnection> socket) = 0;
     /*
        virtual void receive_from_server(Client &client, std::shared_ptr<sockpp::tcp_connector> connector) = 0;
*/


    };
}


