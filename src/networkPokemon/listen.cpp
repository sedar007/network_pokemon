#include "pch.h"

namespace pokemon {

    Listen::Listen(const in_port_t port, const std::shared_ptr<Node_Info> node_info, peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
    : NetworkNode(port, node_info, peers, images_repository, storage)  {
        auto listenThread = connect();
        listenThread.detach();
    }

    std::thread Listen::connect() {
        return std::thread([this] { listening(); });
    }

    void Listen::listening() {
        getTrace().print(std::clog, std::format(MSG_LISTENING_START, std::format(MSG_NODE_ID, getPort(), LISTEN)));

        // Create acceptor
        sockpp::tcp_acceptor acc(getPort());
        if (!acc) {
            std::cerr << "listening:: error creating acceptor : " << getPort() <<std::endl;
            std::cerr << "Error: " << acc.last_error_str() << std::endl;
         //   getTrace().print(std::cerr, std::format(MSG_LISTENING_ERROR_CREATING_ACCEPTOR, std::format(MSG_NODE_ID, getPort(), LISTEN)));
            throw std::runtime_error(MSG_ERROR_CREATING_ACCEPTOR);
        }

        while (true) {
            std::cout << "listening:: listening... port: "<<getPort()<< std::endl;
        //    getTrace().print(std::cerr, std::format(MSG_LISTENING_AWAITING_CONNECTION, std::format(MSG_NODE_ID, getPort(), LISTEN), acc.last_error_str()));
            std::unique_ptr<sockpp::tcp_socket> sock = std::make_unique<sockpp::tcp_socket>(acc.accept()) ;

            if (!sock) {
                std::cerr << "listening:: failed to accept new connection : " << getPort() <<std::endl;
          //      getTrace().print(std::cerr, std::format(MSG_LISTENING_ERROR_ACCEPTING_ACCEPTOR, std::format(MSG_NODE_ID, getPort(), LISTEN), acc.last_error_str()));
                std::this_thread::sleep_for(threadSleep_s(LISTEN_ERROR_CONNECTION_SLEEP_RANGE_BEGIN, LISTEN_ERROR_CONNECTION_SLEEP_RANGE_END));
            }
            else {
              //  getTrace().print(std::clog, std::format(MSG_LISTENING_ACCEPTING_CONNECTION, std::format(MSG_NODE_ID, getPort(), LISTEN)));

                std::cerr << "listening:: new connection : " << getPort() << " from: " << sock->peer_address() <<std::endl;
                std::shared_ptr<sockpp::tcp_socket> sharedSock = std::move(sock);
                auto runServerTask = [this, s = sharedSock]() {
                    session ss(getPort(), get_node_info(), get_peer_registry(), get_images_repository(), get_storage());
                    ss.process(s);
                };
                enqueue_thread(std::move(runServerTask));
            }
        }
    }
}
