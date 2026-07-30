#include "../pch.h"

namespace pokemon {

    void alive_command::send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) {
        if (!socket) {
            return;
        }
        send_alive_ack(socket, *ss.get_node_info());
        socket->shutdown();
    }

    void alive_command::send_alive_ack(const std::shared_ptr<tcp::IConnection>& socket, const Node_Info item) noexcept {
        command::send_item<Node_Info, Node_Packet>(socket, item);
    }

    void alive_command::receive_from_server(Client& client, std::shared_ptr<tcp::tcp_connector> connector) {
        if (connector == nullptr || !(*connector)) {
            return;
        }
        receive_alive_ack(client, connector);
        connector->shutdown();
    }

    void alive_command::receive_alive_ack(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector) {

        auto packet_opt = command::receive_item<Node_Packet>(connector);
        if (!packet_opt.has_value()) {
            return;
        }

        const Node_Info node = Node_Info::from_packet(packet_opt.value());
        if (node.get_ip().empty() || node.get_port() == 0) {
            return;
        }

        client.get_peer_registry().set_node_alive(node.get_ip(), node.get_port(), true);
        client.getTrace().print(std::clog, std::format("{}:{} is up", node.get_ip(), node.get_port()));
    }


}
