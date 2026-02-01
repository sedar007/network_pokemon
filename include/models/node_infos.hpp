#pragma once

namespace pokemon {

    class NETWORK_POKEMON_MODELS_API Node_Info {
        public:
            Node_Info() noexcept = default;

            static constexpr std::string_view DEFAULT_NODE_NAME = "NetworkPokemonNode";
            static constexpr std::string_view NODE_PORT_KEY = "nodePort";
            static constexpr std::string_view NODE_NAME_KEY = "nodeName";

            inline void set_name(std::string_view node_name) noexcept {
                nodeName_s = node_name;
            }
            [[nodiscard]] inline std::string_view get_name() const noexcept {
                return nodeName_s;
            }

            inline void set_port(int port) noexcept {
                port_i = port;
            }
            [[nodiscard]] inline int get_port() const noexcept {
                return port_i;
            }

    private:
        std::string nodeName_s;
        int port_i;
    };
}
