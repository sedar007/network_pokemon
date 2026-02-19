

#pragma once

namespace pokemon::tcp {

    class SockppConnection : public IConnection {

        public:
            SockppConnection(std::unique_ptr<sockpp::tcp_socket> sock)
            : m_sock(std::move(sock)) {}

        // On implémente les méthodes de l'interface en utilisant m_sock
       /* ssize_t read(void* buf, size_t n) override {
            return m_sock->read(buf, n);
        }

        ssize_t write(const void* buf, size_t n) override {
            return m_sock->write(buf, n);
        }

        std::string peer_address() const override {
            return m_sock->peer_address().to_string(); // Ajuste selon la méthode de sockpp
        }*/

        private:
            std::unique_ptr<sockpp::tcp_socket> m_sock;
    };

}






