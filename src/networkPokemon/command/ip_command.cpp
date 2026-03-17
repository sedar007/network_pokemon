#include "../pch.h"

namespace pokemon {

    void ip_command::send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) {
        if (!socket) {
            return;
        }
        send_nodes_list(socket, ss.get_peer_registry().get_nodes());
        socket->shutdown();
    }

    void ip_command::receive_from_server(Client& client, std::shared_ptr<tcp::tcp_connector> connector) {
        if (connector == nullptr || !(*connector)) {
            return;
        }
            receive_nodes_list(client, connector);
           connector->shutdown();
    }

    void ip_command::send_nodes_list(const std::shared_ptr<tcp::IConnection>& socket, const std::vector<Node_Info>& nodes) noexcept{
        command::send_list<Node_Info, Node_Packet>(socket, nodes);
    }

    void ip_command::receive_nodes_list(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector) {

        auto packet_buffer_opt = command::receive_list<Node_Packet>(connector);
        if (!packet_buffer_opt.has_value()) {
            return;
        }

        auto const& packet_buffer = packet_buffer_opt.value();

        for (const auto& packet : packet_buffer) {

            std::string id = command::safe_string(packet.id, sizeof(packet.id));
            std::string name = command::safe_string(packet.name, sizeof(packet.name));
            std::string ip = command::safe_string(packet.ip, sizeof(packet.ip));

            // Big Endian -> Little Endian
            const auto port = ntohs(packet.port);

            if (ip.empty() || port == 0) continue;

            Node_Info node(id, name, ip, port);
            client.get_peer_registry().add_node(node);
            client.get_storage()->addNodeToSavedList(node);
        }
    }
}
