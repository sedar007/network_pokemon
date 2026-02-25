#include "pch.h"


using namespace std::chrono_literals;
namespace pokemon {

    session::session(in_port_t port, const std::shared_ptr<Node_Info> node_info,
        peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
        : NetworkNode(port, node_info, peers, images_repository, storage)
    {
        initCommands();
    }

    int session::process(std::shared_ptr<tcp::IConnection> socket) {
        tcp::ClientNetHelper<session> helper;

        auto& dispatcher = get_dispatcher();
       return helper.server_send_response(*this, std::move(socket),  dispatcher);
    }
}
