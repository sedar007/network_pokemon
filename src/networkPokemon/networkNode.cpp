#include "pch.h"

namespace pokemon {
    NetworkNode::NetworkNode(const in_port_t port) noexcept
    : port_s(port)
    , thread_pool(std::thread::hardware_concurrency()) {
    }

}
