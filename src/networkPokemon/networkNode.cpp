#include "pch.h"

namespace pokemon {
    NetworkNode::NetworkNode(const in_port_t port, const std::shared_ptr<Node_Info> node_info, peer_registry& peers_registry, image_repository& images_repository) noexcept
    : port_s(port)
    , thread_pool(std::thread::hardware_concurrency())
    , node_info_ptr(node_info)
    , peers_registry_(peers_registry)
    , images_repository_(images_repository){
        initCommands();
    }

}
