#include "pch.h"

namespace pokemon {
    Image::Image(std::string_view name, std::string_view extension, std::string_view hash,
        std::string_view owner, std::string_view size, std::string_view size_unit ) noexcept
        : name_s(name)
        , extension_s(extension)
        , hash_s(hash)
        , owner_s(owner)
        , size_s(size)
        , size_unit_s(size_unit){
    }

    Image_Packet Image::to_packet(const Image &image_info) noexcept {
        Image_Packet packet {};
        std::memset(&packet, 0, sizeof(Image_Packet));

        image_info.get_name().copy(packet.name, sizeof(packet.name) - 1);
        image_info.get_extension().copy(packet.extension, sizeof(packet.extension) - 1);
        image_info.get_hash().copy(packet.hash, sizeof(packet.hash) - 1);
        image_info.get_owner().copy(packet.owner, sizeof(packet.owner) - 1);
        image_info.get_size().copy(packet.size, sizeof(packet.size) - 1);
        image_info.get_size_unit().copy(packet.size_unit, sizeof(packet.size_unit) - 1);

        return packet;
    }

    Image Image::from_packet(const Image_Packet &packet) noexcept {
        const std::string name = safe_string(packet.name, sizeof(packet.name));
        const std::string extension = safe_string(packet.extension, sizeof(packet.extension));
        const std::string hash = safe_string(packet.hash, sizeof(packet.hash));
        const std::string owner = safe_string(packet.owner, sizeof(packet.owner));
        const std::string size = safe_string(packet.size, sizeof(packet.size));
        const std::string size_unit = safe_string(packet.size_unit, sizeof(packet.size_unit));

        return Image(name, extension, hash, owner, size, size_unit);
    }

}
