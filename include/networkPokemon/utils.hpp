#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API utils {

    public:
        utils() = default;
        [[nodiscard]] static std::string generate_uuid_v4() noexcept;

    };

} // namespace pokemon
