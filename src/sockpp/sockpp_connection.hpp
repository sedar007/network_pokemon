

#pragma once

namespace pokemon::tcp {

    class SockppConnection : public IConnection {

        public:
            SockppConnection(std::unique_ptr<sockpp::tcp_socket> sock)
            : m_sock(std::move(sock)) {}

            bool is_open() const override {

                return m_sock && m_sock->is_open();
            }

            std::string address() const override {
                return m_sock->address().to_string();
            }

            ssize_t read(void* buf, size_t n) override {
                return m_sock->read(buf, n);
            }

            ssize_t write(const void* buf, size_t n) override {
                return m_sock->write(buf, n);
            }

        private:
            std::unique_ptr<sockpp::tcp_socket> m_sock;
    };

}






