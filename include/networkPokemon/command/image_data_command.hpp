#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API image_data_command : public tcp::INetworkSessionCommand<session>, public tcp::INetworkClientCommand<Client> {
        public:
            void send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) override;
            void receive_from_server(Client &client, std::shared_ptr<tcp::tcp_connector> connector) override;


    private:
        static void send_image(const std::shared_ptr<tcp::IConnection>& socket, const image_cache image_cache) noexcept;
        static void receive_image(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector);

    };
}
