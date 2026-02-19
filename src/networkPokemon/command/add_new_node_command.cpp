#include "../pch.h"

namespace pokemon {

    void add_new_node_command::send_to_client(session& ss, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }

        const Node_Packet packet = Node_Info::to_packet(*ss.get_node_info());

        const size_t total_bytes = sizeof(Node_Packet);
        const std::string header = Utils::formatted_number(total_bytes);

        socket->write(header.data(), header.size());
        socket->write(reinterpret_cast<const char*>(&packet), total_bytes);
        socket->shutdown(SHUT_RDWR);
    }


    void add_new_node_command::receive_from_server(Client& client, std::shared_ptr<sockpp::tcp_connector> connector) {

        size_t total_bytes = Utils::get_total_bytes_from_connector(connector);

        if (total_bytes != sizeof(Node_Packet)) {
            connector->shutdown(SHUT_RDWR);
            return;
        }

        Node_Packet packet {};
        if (!Utils::read_exact(connector, reinterpret_cast<char*>(&packet), sizeof(Node_Packet))) {
            connector->shutdown(SHUT_RDWR);
            return;
        }

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


        const Node_Info node_info = Node_Info::from_packet(packet);

        if (!ip.empty() && port != 0) {
            Node_Info node(id, name, ip, port);
            client.get_peer_registry().add_node(node);
         //   client.get_storage()->addNodeToSavedList(node);
        }

         connector->shutdown(SHUT_RDWR);
    }

}
