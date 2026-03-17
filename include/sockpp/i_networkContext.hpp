#pragma once

namespace pokemon::tcp {

    class NETWORK_SOCKPP_POKEMON_API INetworkContext {
    public:
       virtual ~INetworkContext() = default;
    };
}
