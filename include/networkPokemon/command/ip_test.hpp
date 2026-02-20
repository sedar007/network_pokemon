#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API ip_test : public tcp::INetworkCommand<session> {
        public:
        void send_to_client([[maybe_unused]] session& ss, [[maybe_unused]] std::shared_ptr<tcp::IConnection> socket) override {
            const std::string header = "ok";
            socket->write(header.data(), header.size());
            std::cout << "ok";
        }




    private:


    };
}
