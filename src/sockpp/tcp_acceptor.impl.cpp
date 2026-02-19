#include "pch.h"

namespace pokemon::tcp{

    tcp_acceptor::impl::impl(in_port_t port) noexcept{
        m_acceptor = std::make_unique<sockpp::tcp_acceptor>(port);
    }

    tcp_acceptor::impl::~impl(){
    }

}
