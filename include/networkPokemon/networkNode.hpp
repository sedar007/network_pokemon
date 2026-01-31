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
        NetworkNode(const in_port_t port) noexcept;

        [[nodiscard]] inline in_port_t getPort() const noexcept {
            return port_s;
        }


        [[nodiscard]] inline ResourceManager& getRessource() const noexcept {
            return resourceManager;
        }

        [[nodiscard]] inline Trace& getTrace() const noexcept {
            return trace;
        }



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


    };
}
