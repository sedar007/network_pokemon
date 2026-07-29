#pragma once
#include "const.hpp"

namespace pokemon {
    class NETWORK_POKEMON_API Utils {
    public:
        Utils() = default;

        [[nodiscard]] static std::string generate_uuid_v4() noexcept;

        [[nodiscard]] static std::string formatted_number(size_t number) noexcept;

        [[nodiscard]] static std::string safe_string(const char *data, size_t max_len) noexcept;


        template<typename T>
        [[nodiscard]] static size_t get_total_bytes_from_connector(const std::shared_ptr<T> &conn) {
            char sizeHeader[FORMATTED_NUMBER_SIZE];

            if (!Utils::read_exact(conn, reinterpret_cast<std::byte *>(sizeHeader), FORMATTED_NUMBER_SIZE)) {
                conn->shutdown();
                throw std::runtime_error("Failed to read size header");
            }

            try {
                return std::stoul(std::string(sizeHeader, FORMATTED_NUMBER_SIZE));
            } catch (...) {
                conn->shutdown();
                throw std::runtime_error("Failed to convert size header to number");
            }
        }

        template<typename T>
        [[nodiscard]] static bool read_exact(const std::shared_ptr<T> conn, std::byte *buffer, size_t length) noexcept {
            if (!conn || !buffer){
                std::cout << "Failed to read connection buffer" << std::endl;
                return false;
            }
            if (length <= 0) {
                std::cout << "Invalid length" << std::endl;
                return false;
            }
            return conn->read(buffer, length);
        }
    };
} // namespace pokemon
