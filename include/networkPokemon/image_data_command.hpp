#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API image_data_command : public INetworkCommand {
        public:
            void send_to_client(session& ss, std::shared_ptr<sockpp::tcp_socket> socket) override;
            void receive_from_server(Client &client, std::shared_ptr<sockpp::tcp_connector> connector) override;


    private:
       // std::string getIpsToSend() const;
        ResourceManager &resourceManager = ResourceManager::getInstance();

    };
}
