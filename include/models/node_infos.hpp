#pragma once

namespace pokemon {

    class NETWORK_POKEMON_MODELS_API Node_Info {
        public:
            Node_Info() noexcept = default;

            static constexpr std::string_view DEFAULT_NODE_NAME = "NetworkPokemonNode";

            inline void set_name(std::string_view node_name) noexcept {
                nodeName_s = node_name;
            }
            [[nodiscard]] inline std::string_view get_name() const noexcept {
                return nodeName_s;
            }

    private:
        std::string nodeName_s;
    };
}
