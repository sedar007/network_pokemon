#include "pch.h"

namespace pokemon {
    image_cache::image_cache(std::string_view hash, std::string_view data ) noexcept
        : hash_s(hash)
        , data_s(data)
        {}


    Image_Cache_Packet image_cache::to_packet(const image_cache &image_info) noexcept {
        Image_Cache_Packet packet {};
        std::memset(&packet, 0, sizeof(Image_Cache_Packet));

        image_info.get_data().copy(packet.data, sizeof(packet.data) - 1);
        image_info.get_hash().copy(packet.hash, sizeof(packet.hash) - 1);
        return packet;
    }

    image_cache image_cache::from_packet(const Image_Cache_Packet &packet) noexcept {
        const std::string data = safe_string(packet.data, sizeof(packet.data));
        const std::string hash = safe_string(packet.hash, sizeof(packet.hash));
        return image_cache(hash, data);
    }
}
