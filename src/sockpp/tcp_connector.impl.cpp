#include "pch.h"

namespace pokemon::tcp{

    tcp_connector::impl::impl() noexcept{
        m_connector = std::make_unique<sockpp::tcp_connector>();
    }

    tcp_connector::impl::~impl(){
    }


    bool tcp_connector::impl::connect(std::string_view ip, int port) const noexcept  {
        return m_connector->connect(sockpp::inet_address(std::string(ip), port));
    }

    bool tcp_connector::impl::write(std::string_view data) const noexcept {
        if (!m_connector || !(*m_connector)) {
            return false;
        }
        auto res = m_connector->write(data.data(), data.size());
        return res == static_cast<ssize_t>(data.size());
    }




 //   if (auto res = connector->write(msg.data(), msg.size()); static_cast<size_t>(res) != msg.size()) {

}
