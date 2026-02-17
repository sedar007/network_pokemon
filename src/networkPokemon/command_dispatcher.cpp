#include "pch.h"

namespace pokemon {

    void command_dispatcher::registerCommand(PROTOCOL protocol, std::unique_ptr<INetworkCommand> cmd) {
        m_commands[protocol] = std::move(cmd);
    }

    void command_dispatcher::dispatch_read(Client& client, PROTOCOL protocol, const std::string& payload) {
        auto it = m_commands.find(protocol);
        if (it != m_commands.end()) {
            it->second->read_and_save(client, payload);
        }
    }
    void command_dispatcher::dispatch_write(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        auto it = m_commands.find(protocol);
        if (it != m_commands.end()) {
            it->second->write(ss, protocol, std::move(socket));
        }
    }

    void command_dispatcher::dispatch_client_read(Client& client,PROTOCOL protocol, std::string_view payload) {
        auto it = m_commands.find(protocol);
        if (it != m_commands.end()) {
            it->second->client_read(client, protocol, payload);
        }
    }




}
