#pragma once

namespace pokemon {

    class NETWORK_POKEMON_MODELS_API Model {
        public:
            Model() noexcept = default;
            [[nodiscard]] static std::string safe_string(const char* data, size_t max_len) noexcept;


    private:


    };
}
