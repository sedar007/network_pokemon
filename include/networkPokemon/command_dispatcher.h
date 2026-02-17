#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API command_dispatcher {
    public:
        void registerCommand(PROTOCOL protocol, std::unique_ptr<INetworkCommand> cmd);
        void dispatch_read(Client& client, PROTOCOL protocol, const std::string& payload);
        void dispatch_send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket);
        void dispatch_client_read(Client& client,PROTOCOL protocol, std::string_view payload);


    private:
        std::map<PROTOCOL, std::unique_ptr<INetworkCommand>> m_commands;

    };
}
