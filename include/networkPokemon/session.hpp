#pragma once

namespace pokemon {
    namespace tcp { class IConnection; }
    /**
     * @brief Représente une session pour gérer les connexions entrantes.
     */
    class NETWORK_POKEMON_API  session : public  NetworkNode {
    public:
        /**
         * @brief Constructeur pour initialiser le serveur avec le port spécifié.
         * @param port Le numéro de port sur lequel écouter.
         */
        session(in_port_t port, const std::shared_ptr<Node_Info> node_info, peer_registry& peers,
            image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept;

        /**
         * @brief Traite les données reçues du client.
         * @param socket La socket connectée.
         * @return Un entier représentant le résultat du traitement.
         */
        int process(std::shared_ptr<tcp::IConnection> socket);

    };

}
