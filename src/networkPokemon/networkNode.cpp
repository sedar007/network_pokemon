#include "pch.h"

namespace pokemon {
    NetworkNode::NetworkNode(const in_port_t port, const std::shared_ptr<Node_Info> node_info) noexcept
    : port_s(port)
    , thread_pool(std::thread::hardware_concurrency())
    , node_info_ptr(node_info){
        initCommands();
    }

}
