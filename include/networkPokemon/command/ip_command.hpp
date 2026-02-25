#pragma once

namespace pokemon {

    class session;
    class Client;
    class NETWORK_POKEMON_API ip_command : public tcp::INetworkSessionCommand<session>, public tcp::INetworkClientCommand<Client> {
        public:
            void send_to_client(session& ss, std::shared_ptr<tcp::IConnection> socket) override;
            void receive_from_server(Client &client, std::shared_ptr<tcp::tcp_connector> connector) override;



    private:
        ResourceManager &resourceManager = ResourceManager::getInstance();
       // std::string get_ip_to_send(const std::vector<Node_Info> nodes) const;
    //    void send_nodes_list(std::shared_ptr<tcp::IConnection> socket, const std::vector<Node_Info>& nodes) const noexcept;
        void receive_nodes_list(Client& client, const std::shared_ptr<tcp::tcp_connector> &connector);

    };
}
