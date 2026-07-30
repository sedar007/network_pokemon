#pragma once
#include <string_view>
#include <vector>
#include <optional>
#include <cstdint>

namespace pokemon {

    struct Image_Cache_Packet {
        char hash[65];
        uint32_t chunk_index;
        uint32_t chunk_count;
        uint32_t chunk_size;
        char data[8192];
    };

    class NETWORK_POKEMON_MODELS_API image_cache : public Model {
        public:
            image_cache(std::string_view hash,
                std::string_view data) noexcept;
            image_cache() noexcept = default;

            static constexpr std::string_view IMAGE_CACHE_HASH_KEY = "hash";
            static constexpr std::string_view IMAGE_CACHE_DATA_KEY = "data";
            static constexpr size_t CHUNK_PAYLOAD_SIZE = sizeof(Image_Cache_Packet::data);

            [[nodiscard]] inline std::string_view get_data() const noexcept {
                return data_s;
            }

            [[nodiscard]] inline std::string_view get_hash() const noexcept {
                return hash_s;
            }

            inline void set_data(std::string_view data) noexcept {
                data_s = data;
            }

            inline void set_hash(std::string_view hash) noexcept {
                hash_s = hash;
            }

            [[nodiscard]] static std::vector<Image_Cache_Packet> to_packets(const image_cache &info) noexcept;
            [[nodiscard]] static std::optional<image_cache> from_packets(const std::vector<Image_Cache_Packet> &packets) noexcept;

    private:
        std::string hash_s;
        std::string data_s;

    };
}
