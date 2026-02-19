#include <memory>

namespace pokemon::tcp
{
    tcp_acceptor::tcp_acceptor(const int port)
        : m_pimpl(std::make_unique<impl>(static_cast<in_port_t>(port)))
    {
    }

    tcp_acceptor::~tcp_acceptor()
    {

    }


    tcp_acceptor::tcp_acceptor(tcp_acceptor&&) noexcept = default;
    tcp_acceptor& tcp_acceptor::operator=(tcp_acceptor&&) noexcept = default;

}
