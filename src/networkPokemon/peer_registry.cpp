#include "pch.h"

namespace pokemon {
     void peer_registry::add_node(const Node_Info& nodeInfo) noexcept {
        std::unique_lock lock(mutex_);

        auto it = std::find_if(nodes_.begin(), nodes_.end(),
            [&](const Node_Info& n) { return n.get_id() == nodeInfo.get_id(); });

        if (it == nodes_.end()) {
            nodes_.push_back(nodeInfo);
        }
    }

    std::vector<Node_Info> peer_registry::get_nodes() const {
        std::shared_lock lock(mutex_);
        return nodes_;
    }

    void peer_registry::set_node_alive(std::string_view ip, int port, bool status) {
        std::unique_lock lock(mutex_);
        auto it = std::find_if(nodes_.begin(), nodes_.end(),
            [&](const Node_Info& n) { return n.get_ip() == ip && n.get_port() == port; });

        if (it != nodes_.end()) {
            status = false;
            //it->set_isConnected(status);
        }
    }

    Node_Info peer_registry::find_node_by_id(std::string_view id) const {
        std::shared_lock lock(mutex_);
        auto it = std::find_if(nodes_.begin(), nodes_.end(),
            [&](const Node_Info& n) { return n.get_id() == id; });

        if (it != nodes_.end()) {
            return *it;
        }
        throw std::runtime_error("No such node found");
    }

}
