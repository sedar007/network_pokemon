#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API command_dispatcher {
    public:
        command_dispatcher() = default;
        command_dispatcher(const command_dispatcher&) = delete;
        command_dispatcher& operator=(const command_dispatcher&) = delete;

        void registerCommand(PROTOCOL protocol, std::unique_ptr<INetworkCommand> cmd);
        void dispatch_read(Client& client, PROTOCOL protocol, const std::string& payload);
        void dispatch_send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket);
        void dispatch_client_read(Client& client,PROTOCOL protocol, std::shared_ptr<sockpp::tcp_connector> connector);


    private:
        std::map<PROTOCOL, std::unique_ptr<INetworkCommand>> m_commands;

    };
}
