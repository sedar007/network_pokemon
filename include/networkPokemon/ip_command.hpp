#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API ip_command : public INetworkCommand {
        public:
            void read_and_save(Client& client, const std::string& payload) override;
            void write(Server& server, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) override;
            void client_read(Client &client, PROTOCOL protocol, std::string_view payload) override;


    private:

    };
}
