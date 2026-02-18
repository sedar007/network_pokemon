#pragma once

namespace pokemon::tcp {

    class NETWORK_SOCKPP_POKEMON_API tcp_acceptor {
        public:
            tcp_acceptor(in_port_t port);
            ~tcp_acceptor();
            tcp_acceptor() = delete;
            tcp_acceptor(const tcp_acceptor&) = delete;
            tcp_acceptor& operator=(const tcp_acceptor&) = delete;
            tcp_acceptor(tcp_acceptor&&) noexcept;
            tcp_acceptor& operator=(tcp_acceptor&&) noexcept;

            void test();

        private:
            class impl;
            std::unique_ptr<impl> m_pimpl;

    };
}


