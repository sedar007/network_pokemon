#include <memory>

namespace pokemon::tcp
{
    tcp_connector::tcp_connector() noexcept
        : m_pimpl(std::make_unique<impl>())
    {
    }

    tcp_connector::~tcp_connector()
    {

    }

    bool tcp_connector::connect(std::string_view ip, int port) const noexcept {
        if (!m_pimpl) {
            return false;
        }
        return m_pimpl->connect(ip, port);
    }


    bool tcp_connector::write(std::string_view data) const noexcept {
        if (!m_pimpl) {
            return false;
        }
        return m_pimpl->write(data);
    }

    bool tcp_connector::read(char* buffer, size_t length) const noexcept {
        if (!m_pimpl) {
            return false;
        }
        return m_pimpl->read(buffer, length);
    }

    void tcp_connector::shutdown() const noexcept {
        if (!m_pimpl) {
            return;
        }
        m_pimpl->shutdown();
    }

    tcp_connector::tcp_connector(tcp_connector&&) noexcept = default;
    tcp_connector& tcp_connector::operator=(tcp_connector&&) noexcept = default;

}
