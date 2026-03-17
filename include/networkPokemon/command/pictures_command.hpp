#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API pictures_command : public tcp::INetworkSessionCommand<session>, public tcp::INetworkClientCommand<Client> {
        public:
            void send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) override;
            void receive_from_server(Client &client, std::shared_ptr<tcp::tcp_connector> connector) override;


    private:
        static void send_pictures_list(const std::shared_ptr<tcp::IConnection>& socket, const std::vector<Image>& images) noexcept;
        static void receive_pictures_list(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector);


    };
}
