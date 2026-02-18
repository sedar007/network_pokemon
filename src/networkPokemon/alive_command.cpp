#include "pch.h"

namespace pokemon {

    void alive_command::send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }
        std::string msg = std::format("{};{}", "ALIVE", socket->address().to_string());
        const std::string std_send = std::format("{}{}{}", ss.generateFormattedNumber(msg.size()), ss.protocolToString(protocol), msg);
           std::cout << std_send << std::endl;
        socket->write(&std_send[0], std_send.size());
        socket->shutdown(SHUT_RDWR);
    }

    void alive_command::receive_from_server(Client& client, std::shared_ptr<sockpp::tcp_connector> connector) {
       /* std::stringstream ss(payload.data());
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
        }*/
    }


}
