#include "pch.h"


using namespace std::chrono_literals;
namespace pokemon {

    session::session(in_port_t port, const std::shared_ptr<Node_Info> node_info,
        peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
        : NetworkNode(port, node_info, peers, images_repository, storage){}


    int session::process(std::shared_ptr<tcp::IConnection> socket) {
       return tcp::ClientNetHelper::send_request(std::move(socket));
    }
}
