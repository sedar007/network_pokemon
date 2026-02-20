#include "pch.h"


using namespace std::chrono_literals;
namespace pokemon {

    session::session(in_port_t port, const std::shared_ptr<Node_Info> node_info,
        peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
        : NetworkNode(port, node_info, peers, images_repository, storage){}


    int session::process(std::shared_ptr<tcp::IConnection> socket) {
        tcp::ClientNetHelper<session> helper;

        helper.get_dispatcher().registerCommand(tcp::PROTOCOL::GET_IPS, std::make_unique<ip_test>());
       return helper.send_request(*this, std::move(socket));
    }
}
