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

        const Node_Info node = Node_Info::from_packet(packet_buffer_opt.value());
        if (node.get_ip().empty() || node.get_port() == 0) {
            return;
        }

        client.get_peer_registry().add_node(node);
        client.get_storage()->addNodeToSavedList(node);
    }

}


