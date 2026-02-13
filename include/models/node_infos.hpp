#pragma once

namespace pokemon {

    class NETWORK_POKEMON_MODELS_API Node_Info {
        public:
            Node_Info(std::string_view nodeName, std::string_view ip, int port) noexcept;
            Node_Info() noexcept = default;


            static constexpr std::string_view DEFAULT_NODE_NAME = "NetworkPokemonNode";
            static constexpr std::string_view NODE_PORT_KEY = "nodePort";
            static constexpr std::string_view NODE_NAME_KEY = "nodeName";
            static constexpr std::string_view NODE_IP_KEY = "nodeIp";

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

            inline void set_ip(std::string_view ip) noexcept {
                ip_s = ip;
            }
            [[nodiscard]] inline std::string_view get_ip() const noexcept {
                return ip_s;
            }

            [[nodiscard]] inline bool get_isConnected() const noexcept {
                return isConnected_b;
            }
            inline void set_isConnected(bool isConnected) noexcept {
                isConnected_b = isConnected;
            }

        friend std::ostream &operator<<(std::ostream &os, const Node_Info &node) {
                os << "Node Information:\n"
                   << "  Name: " << node.get_name() << "\n"
                   << "  IP: " << node.get_ip() << "\n"
                   << "  Port: " << node.get_port() << "\n";
                   //<< "  Peer Count: " << resourceManager.getNodesInfoList().size();
                return os;
        }

        friend bool operator==(const Node_Info &lhs, const Node_Info &rhs) noexcept {
            return lhs.get_name() == rhs.get_name() &&
                   lhs.get_ip() == rhs.get_ip() &&
                   lhs.get_port() == rhs.get_port();
            }

    private:
        std::string nodeName_s;
        std::string ip_s;
        int port_i;
        bool isConnected_b;


        bool is_node_online(std::string_view ip) const noexcept;

    };
}
