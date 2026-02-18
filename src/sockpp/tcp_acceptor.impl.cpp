#include "pch.h"

namespace pokemon::tcp{

    tcp_acceptor::impl::impl(in_port_t port) noexcept{
        m_acceptor = std::make_unique<sockpp::tcp_acceptor>(port);
    }

    tcp_acceptor::impl::~impl(){
    }

    void tcp_acceptor::impl::test() {
        std::cout << "test" << std::endl;
    }
}
