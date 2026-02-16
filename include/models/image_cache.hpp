#pragma once
#include <string_view>

namespace pokemon {

    class NETWORK_POKEMON_MODELS_API image_cache {
        public:
            image_cache(std::string_view hash,
                std::string_view data) noexcept;
            image_cache() noexcept = default;

            static constexpr std::string_view IMAGE_CACHE_HASH_KEY = "hash";
            static constexpr std::string_view IMAGE_CACHE_DATA_KEY = "data";

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

    private:
        std::string hash_s;
        std::string data_s;

    };
}
