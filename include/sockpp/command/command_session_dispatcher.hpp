#pragma once

namespace pokemon::tcp {

    template <typename T>
    class NETWORK_SOCKPP_POKEMON_API command_session_dispatcher {
    public:
        command_session_dispatcher() = default;
        command_session_dispatcher(const command_session_dispatcher&) = delete;
        command_session_dispatcher& operator=(const command_session_dispatcher&) = delete;


        void registerCommand(PROTOCOL protocol, std::unique_ptr<INetworkSessionCommand<T>> cmd) {
            m_commands[protocol] = std::move(cmd);
        }

        void dispatch_send_to_client(T &context, PROTOCOL protocol, std::shared_ptr<tcp::IConnection> socket) {
            auto it = m_commands.find(protocol);
            if (it != m_commands.end()) {
                it->second->send_to_client(context, std::move(socket));
            }
        }


    private:
        std::map<PROTOCOL, std::unique_ptr<INetworkSessionCommand<T>>> m_commands;

    };
}
