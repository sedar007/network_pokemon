#pragma once

namespace pokemon {

    class Client;
    class Server;

    class NETWORK_POKEMON_API INetworkCommand {
    public:
        virtual ~INetworkCommand() = default;

        virtual void read_and_save(Client &client, const std::string &payload) = 0;
        virtual void write(Server& server, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) = 0;
        virtual void client_read(Client &client, PROTOCOL protocol, std::string_view payload) = 0;


    };
}


