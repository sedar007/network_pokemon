#pragma once

namespace pokemon::tcp {

    class NETWORK_SOCKPP_POKEMON_API tcp_connector {
        public:
            tcp_connector() noexcept;
            ~tcp_connector();
            tcp_connector(const tcp_connector&) = delete;
            tcp_connector& operator=(const tcp_connector&) = delete;
            tcp_connector(tcp_connector&&) noexcept;
            tcp_connector& operator=(tcp_connector&&) noexcept;


            [[nodiscard]] bool connect(std::string_view ip, int port) const noexcept;
            [[nodiscard]] bool write(std::string_view data) const noexcept;
            [[nodiscard]] bool read(std::byte* buffer, size_t length) const noexcept;
            void shutdown() const noexcept;

            explicit operator bool() const noexcept {
                return static_cast<bool>(m_pimpl);
            }

        private:
            class impl;
            std::unique_ptr<impl> m_pimpl;

    };
}


