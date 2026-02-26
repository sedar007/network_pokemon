#include "pch.h"

namespace pokemon::tcp{

    tcp_connector::impl::impl() noexcept{
        m_connector = std::make_unique<sockpp::tcp_connector>();
    }

    tcp_connector::impl::~impl(){
    }


    bool tcp_connector::impl::connect(const std::string& ip, int port) const noexcept  {
        if (!m_connector) {
            return false;
        }
        if (!is_valid_ip_address(ip)) {
            return false;
        }
        return m_connector->connect(sockpp::inet_address(ip, static_cast<in_port_t>(port)));
    }

    bool tcp_connector::impl::write(std::string_view data) const noexcept {
        if (!m_connector || !(*m_connector)) {
            return false;
        }
        auto res = m_connector->write(data.data(), data.size());
        return res == static_cast<ssize_t>(data.size());
    }


    bool tcp_connector::impl::read(std::byte* buffer, size_t length) const noexcept {
        if (!m_connector || !(*m_connector)) {
            return false;
        }

        size_t total_read = 0;
        while (total_read < length) {
            ssize_t n = m_connector->read(reinterpret_cast<char*>(buffer) + total_read, length - total_read);
            if (n <= 0) return false;
            total_read += n;
        }
        return true;
    }

    void tcp_connector::impl::shutdown() const noexcept {
        if (!m_connector || !(*m_connector)) {
            return;
        }
        m_connector->shutdown(SHUT_RDWR);
    }



 //   if (auto res = connector->write(msg.data(), msg.size()); static_cast<size_t>(res) != msg.size()) {

}
