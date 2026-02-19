#include "pch.h"

namespace pokemon {
    Node_Info::Node_Info(std::string_view id, std::string_view nodeName, std::string_view ip, int port ) noexcept
        : id_s(id)
        , nodeName_s(nodeName)
        , ip_s(ip)
        , port_i(port){
    }

    Node_Packet Node_Info::to_packet(const Node_Info &node_info) noexcept {
        Node_Packet packet {};
        std::memset(&packet, 0, sizeof(Node_Packet));

        node_info.get_id().copy(packet.id, sizeof(packet.id) - 1);
        node_info.get_name().copy(packet.name, sizeof(packet.name) - 1);
        node_info.get_ip().copy(packet.ip, sizeof(packet.ip) - 1);

        packet.port = htons(static_cast<uint16_t>(node_info.get_port()));

        return packet;
    }

    Node_Info Node_Info::from_packet(const Node_Packet &packet) noexcept {
        const std::string id = safe_string(packet.id, sizeof(packet.id));
        const std::string name = safe_string(packet.name, sizeof(packet.name));
        const std::string ip = safe_string(packet.ip, sizeof(packet.ip));
        // Big Endian -> Little Endian
        const auto port = ntohs(packet.port);

        return Node_Info(id, name, ip, port);
    }
}
