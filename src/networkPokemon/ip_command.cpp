#include "pch.h"

namespace pokemon {
    void ip_command::read_and_save(Client& client, const std::string& payload) {

    }

    std::string ip_command::get_ip_to_send(const std::vector<Node_Info> nodes) const {
        std::string str;
        for (const auto &node: nodes)
            str += std::format("{}_{}_{}_{};", node.get_id(), node.get_name(), node.get_ip(), node.get_port());
        return str;
    }

    void ip_command::write(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }

        std::string msg = get_ip_to_send(ss.get_peer_registry().get_nodes());
        const std::string std_send = std::format("{}{}{}", ss.generateFormattedNumber(msg.size()), ss.protocolToString(protocol), msg);
        std::cout << std_send << std::endl;
        socket->write(&std_send[0], std_send.size());
        socket->shutdown(SHUT_RDWR);
    }

    void ip_command::client_read(Client& client, PROTOCOL protocol, std::string_view payload) {
        std::stringstream ss(payload.data());
        std::string data;

        while (std::getline(ss, data, ';')) {
            std::string id_str, name_str, ip_str, port_str;
            std::stringstream ss2(data);

            std::getline(ss2, id_str, '_');
            std::getline(ss2, name_str, '_');
            std::getline(ss2, ip_str, '_');
            std::getline(ss2, port_str);

            try {
                Node_Info nodeInfo(id_str, name_str, ip_str, std::stoi(port_str));
                client.get_peer_registry().add_node(nodeInfo);
                client.get_storage()->addNodeToSavedList(nodeInfo);
            } catch (...) {
            }
        }
    }
}
