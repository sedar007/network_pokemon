#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API Utils {

    public:
        Utils() = default;
        [[nodiscard]] static std::string generate_uuid_v4() noexcept;
        [[nodiscard]] static std::string formatted_number(size_t number) noexcept;
        [[nodiscard]] static bool read_exact(std::shared_ptr<sockpp::tcp_connector> connector, char* buffer, size_t length) noexcept;


    };

} // namespace pokemon
