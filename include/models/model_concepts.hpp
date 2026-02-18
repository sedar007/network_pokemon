#pragma once

namespace pokemon {
    template<typename T, typename PacketType>
    concept Packetable = requires(const T& const_model, const PacketType& const_packet) {
        { const_model.to_packet() } -> std::same_as<PacketType>;
        { T::from_packet(const_packet) } -> std::same_as<T>;
    };
}
