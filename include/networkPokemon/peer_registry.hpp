#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API peer_registry {
    public:

        peer_registry() noexcept = default;

         void add_node(const Node_Info& nodeInfo) noexcept;

        std::vector<Node_Info> get_nodes() const;


        Node_Info find_node_by_id(std::string_view id) const;

        void set_node_alive(std::string_view ip, int port, bool status);
        void printNodesList(std::ostream &os) const {
            /*std::lock_guard<std::mutex> lock(mutex);
            os <<std::endl;
            os << " -- Liste des nodes -- " << std::endl;
            for (auto &node: nodes_)
                os << node << std::endl;
            os << "nombres totals: " + std::to_string(nodes_.size()) << std::endl <<std::endl;*/
        }


    private:
        mutable std::shared_mutex mutex_;
        std::vector<Node_Info> nodes_;
    };

}

