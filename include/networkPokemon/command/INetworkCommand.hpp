#pragma once

namespace pokemon {

    class Client;
    class session;

    class NETWORK_POKEMON_API INetworkCommand {
    public:
        virtual ~INetworkCommand() = default;

        virtual void read_and_save(Client &client, const std::string &payload) = 0;
        virtual void send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) = 0;
        virtual void receive_from_server(Client &client, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_connector> connector) = 0;



    };
}


