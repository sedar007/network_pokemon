#pragma once

namespace pokemon {

    class session;
    class Client;

    class NETWORK_POKEMON_API alive_command : public tcp::INetworkSessionCommand<session>, public tcp::INetworkClientCommand<Client> {
        public:
            void send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) override;
            void receive_from_server(Client &client, std::shared_ptr<tcp::tcp_connector> connector) override;

    private:
        static void send_alive_ack(const std::shared_ptr<tcp::IConnection>& socket, const Node_Info item) noexcept;
        static void receive_alive_ack(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector);

    };
}
