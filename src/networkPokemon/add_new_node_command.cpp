#include "pch.h"

namespace pokemon {
    void add_new_node_command::read_and_save(Client& client, const std::string& payload) {

    }

    void add_new_node_command::write(Server& server, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }
        std::string msg = std::format("{};{}", server.get_id(), socket->address().to_string());
        const std::string std_send = std::format("{}{}{}", server.generateFormattedNumber(msg.size()), server.protocolToString(protocol), msg);
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

        resourceManager.addNode(node_info);
        auto node_List = Json::loadJson<std::vector<Node_Info>>(resourceManager.get_path(), Node::NODE_LIST_FILE);
        if (node_List.has_value()) {
            auto nodes = node_List.value();
            nodes.push_back(node_info);
            Json::saveJson<std::vector<Node_Info>>(resourceManager.get_path(), Node::NODE_LIST_FILE, nodes);
        } else {
            std::vector<Node_Info> nodes;
            nodes.push_back(node_info);
            Json::saveJson<std::vector<Node_Info>>(resourceManager.get_path(), Node::NODE_LIST_FILE, nodes);
        }

        int port = std::atoi(port_str.c_str());
  /*      if (alive == "ALIVE") {
            client.getRessource().set_node_alive(ip, port, true);
            client.getTrace().print(std::cout, std::format("{}:{} is up", ip ,port_str));
        }*/
    }

    inline void to_json(nlohmann::json& j, const Node_Info& n) {
        j = nlohmann::json{
                {Node_Info::NODE_ID_KEY, n.get_id()},
                {Node_Info::NODE_NAME_KEY, n.get_name()},
                {Node_Info::NODE_PORT_KEY, n.get_port()},
                {Node_Info::NODE_IP_KEY, n.get_ip()}
        };
    }

    inline void from_json(const nlohmann::json& j, Node_Info& n) {
        n.set_id(j.at(Node_Info::NODE_ID_KEY).get<std::string>());
        n.set_name(j.at(Node_Info::NODE_NAME_KEY).get<std::string>());
        n.set_port(j.at(Node_Info::NODE_PORT_KEY).get<int>());
        n.set_ip(j.at(Node_Info::NODE_IP_KEY).get<std::string>());
    }
}
