#include "../pch.h"

namespace pokemon::tcp {



    /*void command_dispatcher::registerCommand(PROTOCOL protocol, std::unique_ptr<INetworkCommand> cmd) {
        m_commands[protocol] = std::move(cmd);
    }



    void command_dispatcher::dispatch_client_read(Client& client,PROTOCOL protocol, std::shared_ptr<sockpp::tcp_connector> connector) {
        auto it = m_commands.find(protocol);
        if (it != m_commands.end()) {
            it->second->receive_from_server(client, std::move(connector));
        }
    }*/




}
