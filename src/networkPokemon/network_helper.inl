#pragma once
#include "network_helper.hpp"

namespace pokemon {

    template <typename T, typename PacketType>
    requires Packetable<T, PacketType>
    void network_helper::send_list(std::shared_ptr<sockpp::tcp_socket> socket, const std::vector<T>& items) {
        if (items.empty()) return;

        std::vector<PacketType> packet_buffer;
        packet_buffer.reserve(items.size());

        for (const auto& item : items) {
            packet_buffer.push_back(item.to_packet());
        }

        const size_t total_bytes = packet_buffer.size() * sizeof(PacketType);
        const std::string header = Utils::formatted_number(total_bytes);

        socket->write(header.data(), header.size());
        socket->write(reinterpret_cast<const char*>(packet_buffer.data()), total_bytes);
    }

}
