#include "../pch.h"

namespace pokemon {

    void add_new_node_command::send_to_client([[maybe_unused]] session& ss,[[maybe_unused]] const std::shared_ptr<tcp::IConnection>& socket) {
        if (!socket) {
            return;
        }
        send_node(socket, *ss.get_node_info());
        socket->shutdown();
    }

    void add_new_node_command::send_node(const std::shared_ptr<tcp::IConnection>& socket, const Node_Info item) noexcept{
        command::send_item<Node_Info, Node_Packet>(socket, item);
    }

    void add_new_node_command::receive_from_server([[maybe_unused]] Client& client, [[maybe_unused]] std::shared_ptr<tcp::tcp_connector> connector) {
        if (connector == nullptr || !(*connector)) {
            return;
        }
        receive_node(client, connector);
        connector->shutdown();
    }

    void add_new_node_command::receive_node(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector) {

        auto packet_buffer_opt = command::receive_item<Node_Packet>(connector);
        if (!packet_buffer_opt.has_value()) {
            return;
        }

        auto const& packet_buffer = packet_buffer_opt.value();

      /*  auto const& packet_buffer = packet_buffer_opt.value();

        std::string hash = command::safe_string(packet_buffer.hash, sizeof(packet_buffer.hash));
        std::string data = command::safe_string(packet_buffer.data, sizeof(packet_buffer.data));*/
        /*
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
                }*/
    }

}


