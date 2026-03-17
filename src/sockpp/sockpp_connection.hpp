

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


            bool read(std::byte* buffer, size_t length) override {
                if (!m_sock || !(*m_sock)) {
                    return false;
                }

                size_t total_read = 0;
                while (total_read < length) {
                    ssize_t n = m_sock->read(reinterpret_cast<char*>(buffer) + total_read, length - total_read);
                    if (n <= 0) return false;
                    total_read += n;
                }

                return true;
            }

            ssize_t write(const void* buf, size_t n) override {
                return m_sock->write(buf, n);
            }

            void shutdown() override {
                m_sock->shutdown(SHUT_RDWR);
            }

            std::string address_to_string() const override {
                return m_sock->address().to_string();
            }




        private:
            std::unique_ptr<sockpp::tcp_socket> m_sock;
    };

}






