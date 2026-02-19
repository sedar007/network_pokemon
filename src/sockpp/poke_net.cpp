#include <memory>

namespace pokemon::tcp
{
    poke_net::poke_net(const int port)
        : m_pimpl(std::make_unique<impl>(static_cast<in_port_t>(port)))
    {
    }

    poke_net::~poke_net()
    {

    }

    void poke_net::listen(std::function<void(std::unique_ptr<IConnection>)> on_connect) {
        m_pimpl->listen(std::move(on_connect));
    }


    poke_net::poke_net(poke_net&&) noexcept = default;
    poke_net& poke_net::operator=(poke_net&&) noexcept = default;


}


