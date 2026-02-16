#pragma once

/**
 * @file NetworkNode.hpp
 * @brief Definition of the NetworkNode class.
 */

namespace pokemon {

    /**
     * @class NetworkNode
     * @brief Represents a generic node in the peer-to-peer network.
     * * This class serves as a base entity containing the essential network
     * identity (IP and Port) and access to singleton utilities like
     * the Resource Manager and Tracer.
     */
    class NETWORK_POKEMON_API NetworkNode : public Helper {
    public:
        /**
         * @brief Constructs a new Network Node object.
         * * Initializes the node with a specific IP address and port number.
         * This constructor is noexcept and guarantees not to throw exceptions.
         * @param port The network port number.
         */
        NetworkNode(const in_port_t port, const std::shared_ptr<Node_Info> node_info,
            peer_registry& peers_registry, image_repository& images) noexcept;

        inline std::shared_ptr<Node_Info> get_node_info() const noexcept {
            return node_info_ptr;
        }

        [[nodiscard]] inline std::string_view get_id() const noexcept {
            return node_info_ptr->get_id();
        }


        [[nodiscard]] inline in_port_t getPort() const noexcept {
            return port_s;
        }


        [[nodiscard]] inline ResourceManager& getRessource() const noexcept {
            return resourceManager;
        }

        [[nodiscard]] inline Trace& getTrace() const noexcept {
            return trace;
        }

        [[nodiscard]] inline peer_registry& get_peer_registry() const noexcept {
            return peers_registry_;
        }

        [[nodiscard]] inline image_repository& get_images_repository() const noexcept {
            return images_repository_;
        }

        template<class F>
         void enqueue_thread(F&& f) noexcept {
            thread_pool.enqueue(std::forward<F>(f));
        }

        void initCommands() {
            m_dispatcher.registerCommand(PROTOCOL::GET_IPS, std::make_unique<ip_command>());
            //  m_dispatcher.registerCommand(protocolToString(PROTOCOL::GET_PICS), std::make_unique<GetPicsCommand>());
            // m_dispatcher.registerCommand(protocolToString(PROTOCOL::GET_PIC), std::make_unique<GetPicCommand>());
            m_dispatcher.registerCommand(PROTOCOL::GET_ALIVE, std::make_unique<alive_command>());
            m_dispatcher.registerCommand(PROTOCOL::GET_ID, std::make_unique<add_new_node_command>());
        }


        command_dispatcher m_dispatcher;


    private:
        /**
         * @brief The port number associated with this node.
         */
        in_port_t port_s;

        /**
         * @brief Reference to the singleton Resource Manager instance.
         * Used for retrieving game assets or configuration data.
         */
        ResourceManager &resourceManager = ResourceManager::getInstance();

        /**
        * @brief Reference to the singleton Trace instance.
        * Used for logging debug information and network events.
        */
        Trace &trace = Trace::getInstance();

        Thread_pool thread_pool;

        std::shared_ptr<Node_Info> node_info_ptr;

        peer_registry& peers_registry_;
        image_repository& images_repository_;


    };
}
