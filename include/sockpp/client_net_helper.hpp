#pragma once

namespace pokemon::tcp {

    class NETWORK_SOCKPP_POKEMON_API ClientNetHelper {
    public:
         ClientNetHelper() = default;

        [[nodiscard]] static int send_request(std::shared_ptr<tcp::IConnection> socket);


    };
}


