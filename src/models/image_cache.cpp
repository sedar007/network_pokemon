#include "pch.h"
#include <algorithm>
#include <cstring>

namespace pokemon {
    image_cache::image_cache(std::string_view hash, std::string_view data ) noexcept
        : hash_s(hash)
        , data_s(data)
        {}

    std::vector<Image_Cache_Packet> image_cache::to_packets(const image_cache &image_info) noexcept {
        const std::string_view data = image_info.get_data();
        const size_t chunk_count = data.empty() ? 1 : (data.size() + CHUNK_PAYLOAD_SIZE - 1) / CHUNK_PAYLOAD_SIZE;

        std::vector<Image_Cache_Packet> packets;
        packets.reserve(chunk_count);

        for (size_t index = 0; index < chunk_count; ++index) {
            Image_Cache_Packet packet {};
            std::memset(&packet, 0, sizeof(packet));

            image_info.get_hash().copy(packet.hash, sizeof(packet.hash) - 1);
            packet.chunk_index = htonl(static_cast<uint32_t>(index));
            packet.chunk_count = htonl(static_cast<uint32_t>(chunk_count));

            const size_t offset = index * CHUNK_PAYLOAD_SIZE;
            const size_t chunk_size = std::min(CHUNK_PAYLOAD_SIZE, data.size() - offset);
            if (chunk_size > 0) {
                std::memcpy(packet.data, data.data() + offset, chunk_size);
            }
            packet.chunk_size = htonl(static_cast<uint32_t>(chunk_size));

            packets.push_back(packet);
        }

        return packets;
    }

    std::optional<image_cache> image_cache::from_packets(const std::vector<Image_Cache_Packet> &packets) noexcept {
        if (packets.empty()) {
            return std::nullopt;
        }

        const std::string hash = safe_string(packets.front().hash, sizeof(packets.front().hash));
        const uint32_t expected_count = ntohl(packets.front().chunk_count);

        if (hash.empty() || expected_count == 0 || packets.size() != expected_count) {
            return std::nullopt;
        }

        std::vector<const Image_Cache_Packet*> ordered(expected_count, nullptr);

        for (const auto& packet : packets) {
            const std::string packet_hash = safe_string(packet.hash, sizeof(packet.hash));
            const uint32_t index = ntohl(packet.chunk_index);
            const uint32_t count = ntohl(packet.chunk_count);
            const uint32_t chunk_size = ntohl(packet.chunk_size);

            const bool valid = packet_hash == hash
                && count == expected_count
                && index < expected_count
                && chunk_size <= sizeof(packet.data)
                && ordered[index] == nullptr;

            if (!valid) {
                return std::nullopt;
            }
            ordered[index] = &packet;
        }

        std::string data;
        data.reserve(packets.size() * CHUNK_PAYLOAD_SIZE);
        for (const auto* packet : ordered) {
            const uint32_t chunk_size = ntohl(packet->chunk_size);
            data.append(packet->data, chunk_size);
        }

        return image_cache(hash, data);
    }
}
