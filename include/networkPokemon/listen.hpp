#pragma once
#include "networkNode.hpp"
/**
 * @file listen.hpp
 * @brief Définition de la classe Listen.
 */


namespace pokemon {

    /**
     * @brief Classe responsable de l'écoute des connexions sur un port spécifique.
     */
    class NETWORK_POKEMON_API Listen : public NetworkNode {
    public:
        /**
         * @brief Constructeur de la classe Listen.
         * @param port Port sur lequel écouter les connexions.
         */
        Listen(const in_port_t port, const std::shared_ptr<Node_Info> node_info, peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept;

        /**
         * @brief Lance l'écoute des connexions sur un port spécifique dans un thread séparé.
         * @return Un objet std::thread représentant le thread d'écoute.
         */
        [[nodiscard]] std::thread connect();

    private:
        /**
         * @brief Méthode interne pour écouter les connexions entrantes.
         *
         */
        void listening();
    };
}
