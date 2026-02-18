#pragma once

namespace pokemon {

    class NETWORK_POKEMON_MODELS_API Utils_Models {

    public:
        Utils_Models() = default;
        [[nodiscard]] static std::string safe_string(const char* data, size_t max_len) noexcept;

    };

} // namespace pokemon
