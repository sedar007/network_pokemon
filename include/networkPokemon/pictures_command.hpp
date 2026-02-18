#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API pictures_command : public INetworkCommand {
        public:
            void send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) override;
            void receive_from_server(Client &client, std::shared_ptr<sockpp::tcp_connector> connector) override;


    private:
        ResourceManager &resourceManager = ResourceManager::getInstance();
        std::string get_pictures_to_send(const std::vector<Image> images) const;

    };
}
