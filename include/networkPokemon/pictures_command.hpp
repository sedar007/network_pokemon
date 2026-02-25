#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API pictures_command : public tcp::INetworkSessionCommand<session>, public tcp::INetworkClientCommand<Client> {
        public:
            void send_to_client(session& ss, std::shared_ptr<tcp::IConnection> socket) override;
            void receive_from_server(Client &client, std::shared_ptr<tcp::tcp_connector> connector) override;


    private:
        ResourceManager &resourceManager = ResourceManager::getInstance();
        std::string get_pictures_to_send(const std::vector<Image> images) const;

    };
}
