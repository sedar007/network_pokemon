#pragma once

namespace pokemon::tcp {

    template <typename T>
    class NETWORK_SOCKPP_POKEMON_API command_client_dispatcher {
    public:
        command_client_dispatcher() = default;
        command_client_dispatcher(const command_client_dispatcher&) = delete;
        command_client_dispatcher& operator=(const command_client_dispatcher&) = delete;


        void registerCommand(PROTOCOL protocol, std::unique_ptr<INetworkClientCommand<T>> cmd) {
            m_commands[protocol] = std::move(cmd);
        }

        void dispatch_client_read(T &context,PROTOCOL protocol, std::shared_ptr<tcp_connector> connector) {
            auto it = m_commands.find(protocol);
            if (it != m_commands.end()) {
                it->second->receive_from_server(context, std::move(connector));
            }
        }
/*

        void dispatch_read(Client& client, PROTOCOL protocol, const std::string& payload);

        void dispatch_client_read(Client& client,PROTOCOL protocol, std::shared_ptr<sockpp::tcp_connector> connector);
*/

    private:
        std::map<PROTOCOL, std::unique_ptr<INetworkClientCommand<T>>> m_commands;

    };
}
