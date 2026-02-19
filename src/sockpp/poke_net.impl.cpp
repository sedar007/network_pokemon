#include "pch.h"

namespace pokemon::tcp{

    poke_net::impl::impl(in_port_t port) noexcept{
        m_acceptor = std::make_unique<sockpp::tcp_acceptor>(port);
    }

    poke_net::impl::~impl(){
    }

    void poke_net::impl::listen(std::function<void(std::unique_ptr<IConnection>)> on_connect) {

        if (!m_acceptor) {
            //std::cerr << "listening:: error creating acceptor : " << getPort() <<std::endl;
            //std::cerr << "Error: " << acc.last_error_str() << std::endl;
            //   getTrace().print(std::cerr, std::format(MSG_LISTENING_ERROR_CREATING_ACCEPTOR, std::format(MSG_NODE_ID, getPort(), LISTEN)));
           // throw std::runtime_error(MSG_ERROR_CREATING_ACCEPTOR);
        }

        while (true) {
            //std::cout << "listening:: listening... port: "<<getPort()<< std::endl;
            //    getTrace().print(std::cerr, std::format(MSG_LISTENING_AWAITING_CONNECTION, std::format(MSG_NODE_ID, getPort(), LISTEN), acc.last_error_str()));
            std::unique_ptr<sockpp::tcp_socket> sock = std::make_unique<sockpp::tcp_socket>(m_acceptor->accept()) ;


            if (!sock) {
              //  std::cerr << "listening:: failed to accept new connection : " << getPort() <<std::endl;
                //      getTrace().print(std::cerr, std::format(MSG_LISTENING_ERROR_ACCEPTING_ACCEPTOR, std::format(MSG_NODE_ID, getPort(), LISTEN), acc.last_error_str()));
                //std::this_thread::sleep_for(threadSleep_s(LISTEN_ERROR_CONNECTION_SLEEP_RANGE_BEGIN, LISTEN_ERROR_CONNECTION_SLEEP_RANGE_END));
            }
            else {
                //  getTrace().print(std::clog, std::format(MSG_LISTENING_ACCEPTING_CONNECTION, std::format(MSG_NODE_ID, getPort(), LISTEN)));

                std::unique_ptr<IConnection> session = std::make_unique<SockppConnection>(std::move(sock));

                if (on_connect) {
                    on_connect(std::move(session));
                }
            }
        }


    }

}
