#include "pch.h"
#include "sockpp/tcp_acceptor.hpp"

namespace pokemon {

    Server::Server(const in_port_t port, const std::shared_ptr<Node_Info> node_info, peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
    : NetworkNode(port, node_info, peers, images_repository, storage)  {
        auto listenThread = connect();
        listenThread.detach();
    }

    std::thread Server::connect() {
        return std::thread([this] { listening(); });
    }

    void Server::listening() {
        getTrace().print(std::clog, std::format(MSG_LISTENING_START, std::format(MSG_NODE_ID, getPort(), LISTEN)));

        tcp::poke_net poke_net(getPort());
        poke_net.listen([this](std::unique_ptr<tcp::IConnection> conn) {

            std::shared_ptr<tcp::IConnection> sharedConn = std::move(conn);

            auto runServerTask = [this, s = sharedConn]() {
                session ss(getPort(), get_node_info(), get_peer_registry(), get_images_repository(), get_storage());
                ss.process(s);
            };

            enqueue_thread(std::move(runServerTask));
        });
    }

}
