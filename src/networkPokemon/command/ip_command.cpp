#include "../pch.h"

namespace pokemon {

    void ip_command::send_to_client(session& ss, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }

        send_nodes_list(socket, ss.get_peer_registry().get_nodes());
        socket->shutdown(SHUT_RDWR);
    }

    void ip_command::receive_from_server(Client& client, std::shared_ptr<sockpp::tcp_connector> connector) {
        if (connector == nullptr || !(*connector)) {
            return;
        }
        receive_nodes_list(client, connector);
        connector->shutdown(SHUT_RDWR);
    }


    void ip_command::send_nodes_list(std::shared_ptr<sockpp::tcp_socket> socket, const std::vector<Node_Info>& nodes) const noexcept{
        if (nodes.empty()) return;

        std::vector<Node_Packet> packet_buffer;
        packet_buffer.reserve(nodes.size());

        for (const auto& node : nodes) {
            const Node_Packet packet = Node_Info::to_packet(node);
            packet_buffer.push_back(packet);
        }

        const size_t total_bytes = packet_buffer.size() * sizeof(Node_Packet);
        const std::string header = Utils::formatted_number(total_bytes);

        socket->write(header.data(), header.size());
        socket->write(reinterpret_cast<const char*>(packet_buffer.data()), total_bytes);
    }

    void ip_command::receive_nodes_list(Client& client, std::shared_ptr<sockpp::tcp_connector> connector) {

         size_t total_bytes = Utils::get_total_bytes_from_connector(connector);

        if (total_bytes == 0 || total_bytes % sizeof(Node_Packet) != 0) {
            if (total_bytes == 0) return;
            connector->shutdown(SHUT_RDWR);
            return;
        }

        size_t node_count = total_bytes / sizeof(Node_Packet);
        std::vector<Node_Packet> packet_buffer(node_count);

        char* raw_ptr = reinterpret_cast<char*>(packet_buffer.data());

        if (!Utils::read_exact(connector, raw_ptr, total_bytes)) {
            connector->shutdown(SHUT_RDWR);
            return;
        }

        for (const auto& packet : packet_buffer) {
            auto safe_string = [](const char* data, size_t max_len) {
                size_t len = 0;
                while(len < max_len && data[len] != '\0') len++;
                return std::string(data, len);
            };

            std::string id = safe_string(packet.id, sizeof(packet.id));
            std::string name = safe_string(packet.name, sizeof(packet.name));
            std::string ip = safe_string(packet.ip, sizeof(packet.ip));

            // Big Endian -> Little Endian
            uint16_t port = ntohs(packet.port);

            if (ip.empty() || port == 0) continue;

            Node_Info node(id, name, ip, port);
            client.get_peer_registry().add_node(node);
            client.get_storage()->addNodeToSavedList(node);
        }
    }
}
