#include "pch.h"

namespace pokemon {
    void alive_command::read_and_save(Client& client, const std::string& payload) {
        std::stringstream ss(payload);

    }

    void alive_command::write(Server& server, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }
        std::string msg = std::format("{};{}", "ALIVE", socket->address().to_string());
        const std::string std_send = std::format("{}{}{}", server.generateFormattedNumber(msg.size()), server.protocolToString(protocol), msg);
           std::cout << std_send << std::endl;
        socket->write(&std_send[0], std_send.size());
        socket->shutdown(SHUT_RDWR);
    }

    void alive_command::client_read(Client& client, PROTOCOL protocol, std::string_view payload) {
        std::stringstream ss(payload.data());
        std::string alive;
        std::string ip;
        std::string port_str;

        std::getline(ss, alive, ';');
        std::getline(ss, ip, ':');
        std::getline(ss, port_str);

        int port = std::atoi(port_str.c_str());
        if (alive == "ALIVE") {
            client.get_peer_registry().set_node_alive(ip, port, true);
            client.getTrace().print(std::cout, std::format("{}:{} is up", ip ,port_str));
        }
    }


}
