#include "pch.h"

namespace pokemon {
    void ip_command::read_and_save(Client& client, const std::string& payload) {

    }

    std::string ip_command::getIpsToSend() const {
        std::string str;
        for (const auto &node: resourceManager.getNodesInfoList())
            str += std::format("{}_{}_{};", node.get_name(), node.get_ip(), node.get_port());
        return str;
    }

    void ip_command::write(Server& server, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }

        std::string msg = getIpsToSend();
        const std::string std_send = std::format("{}{}{}", server.generateFormattedNumber(msg.size()), server.protocolToString(protocol), msg);
        std::cout << std_send << std::endl;
        socket->write(&std_send[0], std_send.size());
        socket->shutdown(SHUT_RDWR);
    }

    void ip_command::client_read(Client& client, PROTOCOL protocol, std::string_view payload) {
        std::stringstream ss(payload.data());
        std::string data;

        while (std::getline(ss, data, ';')) {
            std::string name_str, ip_str, port_str;
            std::stringstream ss2(data);

            std::getline(ss2, name_str, '_');
            std::getline(ss2, ip_str, '_');
            std::getline(ss2, port_str);

            try {
                Node_Info nodeInfo(name_str, ip_str, std::stoi(port_str));

                resourceManager.addNode(nodeInfo);
            } catch (...) {
            }
        }
    }
}
