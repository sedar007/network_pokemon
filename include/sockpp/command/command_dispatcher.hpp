#pragma once

namespace pokemon::tcp {

    template <typename T>
    class NETWORK_SOCKPP_POKEMON_API command_dispatcher {
    public:
        command_dispatcher() = default;
        command_dispatcher(const command_dispatcher&) = delete;
        command_dispatcher& operator=(const command_dispatcher&) = delete;


        void registerCommand(PROTOCOL protocol, std::unique_ptr<INetworkCommand<T>> cmd) {
            m_commands[protocol] = std::move(cmd);
        }

        void dispatch_send_to_client([[maybe_unused]] T &context, PROTOCOL protocol, [[maybe_unused]] std::shared_ptr<tcp::IConnection> socket) {
            auto it = m_commands.find(protocol);
            if (it != m_commands.end()) {
                it->second->send_to_client(context, std::move(socket));
            }
        }
/*

        void dispatch_read(Client& client, PROTOCOL protocol, const std::string& payload);

        void dispatch_client_read(Client& client,PROTOCOL protocol, std::shared_ptr<sockpp::tcp_connector> connector);
*/

    private:
        std::map<PROTOCOL, std::unique_ptr<INetworkCommand<T>>> m_commands;

    };
}
