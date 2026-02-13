#include "pch.h"

namespace pokemon {
    void ip_command::read_and_save(Client& client, const std::string& payload) {
        std::stringstream ss(payload);
        std::string data;

        auto& resourceManager = client.getRessource();

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

    void ip_command::write(Server& server, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {

    }

    void ip_command::client_read(Client& client, PROTOCOL protocol, std::string_view payload) {

    }
}
