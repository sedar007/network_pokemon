#pragma once

namespace pokemon::tcp {

    class NETWORK_SOCKPP_POKEMON_API poke_net {
        public:
            poke_net(const int port);
            ~poke_net();
            poke_net() = delete;
            poke_net(const poke_net&) = delete;
            poke_net& operator=(const poke_net&) = delete;
            poke_net(poke_net&&) noexcept;
            poke_net& operator=(poke_net&&) noexcept;

            void listen(std::function<void(std::unique_ptr<IConnection>)> on_connect);


    private:
        class impl;
        std::unique_ptr<impl> m_pimpl;

    };
}


