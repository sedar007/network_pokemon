#include "pch.h"

namespace pokemon {
    void add_new_node_command::read_and_save(Client& client, const std::string& payload) {

    }

    void add_new_node_command::send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }
        std::string msg = std::format("{};{}", ss.get_id(), socket->address().to_string());
        const std::string std_send = std::format("{}{}{}", ss.generateFormattedNumber(msg.size()), ss.protocolToString(protocol), msg);
        std::cout << std_send << std::endl;
        socket->write(&std_send[0], std_send.size());
        socket->shutdown(SHUT_RDWR);
    }

    void add_new_node_command::client_read(Client& client, PROTOCOL protocol, std::string_view payload) {
        std::stringstream ss(payload.data());
        std::string id;
        std::string ip;
        std::string port_str;

        std::getline(ss, id, ';');
        std::getline(ss, ip, ':');
        std::getline(ss, port_str);

        Node_Info node_info(id, "unknown", ip, std::atoi(port_str.c_str()));

        client.get_peer_registry().add_node(node_info);
        client.get_storage()->addNodeToSavedList(node_info);
    }

}
