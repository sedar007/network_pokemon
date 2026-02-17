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

    void ip_command::send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
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


    void ip_command::send_nodes_list(std::shared_ptr<sockpp::tcp_socket> socket, const std::vector<Node_Info>& nodes) {
        /*if (nodes.empty()) return;

        // 1. Préparer le buffer de paquets
        std::vector<Node_Packet> packet_buffer;
        packet_buffer.reserve(nodes.size());

        for (const auto& node : nodes) {
            Node_Packet packet;

            // Zéro-init pour la propreté (évite d'envoyer de la RAM poubelle)
            std::memset(&packet, 0, sizeof(Node_Packet));

            // Copie sécurisée : ID
            // On utilise node.get_id() et on limite à la taille du buffer - 1
            std::strncpy(packet.id, node.get_id().data(), sizeof(packet.id) - 1);

            // Copie sécurisée : Nom
            std::strncpy(packet.name, node.get_name().data(), sizeof(packet.name) - 1);

            // Copie sécurisée : IP
            std::strncpy(packet.ip, node.get_ip().data(), sizeof(packet.ip) - 1);

            // Port : Conversion Host-to-Network (Little Endian -> Big Endian)
            packet.port = htons(static_cast<uint16_t>(node.get_port()));

            packet_buffer.push_back(packet);
        }

        // 2. Calculer la taille totale
        size_t total_bytes = packet_buffer.size() * sizeof(Node_Packet);

        // 3. Envoyer le header (ta fonction existante pour formater la taille)
        // Supposons que generateFormattedNumber renvoie une string
        // Attention: ici on envoie la taille en octets, pas le nombre de noeuds
        std::string header = generateFormattedNumber(total_bytes);

        // Écriture Header
        socket->write(header.data(), header.size());

        // 4. Écriture Corps (Les données brutes)
        socket->write(reinterpret_cast<const char*>(packet_buffer.data()), total_bytes);*/
    }

  /*  void receive_nodes_list(Client& client, std::shared_ptr<sockpp::tcp_socket> socket) {
        // 1. Lire la taille (Header) - Supposons FORMATTED_NUMBER_SIZE = 8
        char sizeHeader[8];
        if (socket->read(sizeHeader, 8) != 8) return;

        size_t total_bytes = 0;
        try {
            total_bytes = std::stoul(std::string(sizeHeader, 8));
        } catch(...) { return; }

        // Validation basique
        if (total_bytes == 0 || total_bytes % sizeof(Node_Packet) != 0) return;

        // 2. Allouer le buffer pour tout recevoir d'un coup
        size_t node_count = total_bytes / sizeof(Node_Packet);
        std::vector<Node_Packet> packet_buffer(node_count);

        // 3. Lecture en bloc (plus efficace que lire 1 par 1)
        char* raw_ptr = reinterpret_cast<char*>(packet_buffer.data());
        size_t received = 0;

        while (received < total_bytes) {
            ssize_t n = socket->read(raw_ptr + received, total_bytes - received);
            if (n <= 0) break;
            received += n;
        }

        // 4. Conversion inverse : Packet -> Node_Info
        for (const auto& packet : packet_buffer) {
            // Les char[] se convertissent automatiquement en std::string_view / std::string
            // ntohs : Conversion Network-to-Host
            int port = ntohs(packet.port);

            Node_Info node(packet.id, packet.name, packet.ip, port);

            // Note: isConnected n'est pas transmis, on le définit par défaut
            // node.set_isConnected(true); ou false selon ta logique

            // Ajout à ton registre existant
            client.get_peer_registry().add_node(node);
        }
    } */
}
