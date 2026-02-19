#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API ip_command : public INetworkCommand {
        public:
            void send_to_client(session& ss, std::shared_ptr<sockpp::tcp_socket> socket) override;
            void receive_from_server(Client &client, std::shared_ptr<sockpp::tcp_connector> connector) override;




    private:
        ResourceManager &resourceManager = ResourceManager::getInstance();
        std::string get_ip_to_send(const std::vector<Node_Info> nodes) const;
        void send_nodes_list(std::shared_ptr<sockpp::tcp_socket> socket, const std::vector<Node_Info>& nodes) const noexcept;
        void receive_nodes_list(Client& client, std::shared_ptr<sockpp::tcp_connector> connector);

    };
}
