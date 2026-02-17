#pragma once

namespace pokemon {

    /**
     * @brief Représente un serveur pour gérer les connexions entrantes.
     */
    class NETWORK_POKEMON_API  session : public NetworkNode {
    public:
        /**
         * @brief Constructeur pour initialiser le serveur avec le port spécifié.
         * @param port Le numéro de port sur lequel écouter.
         */
        session(in_port_t port, const std::shared_ptr<Node_Info> node_info, peer_registry& peers,
            image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept;

        /**
         * @brief Démarre le serveur et gère les connexions entrantes.
         * @param socket La socket connectée.
         * @return Un thread qui exécute le serveur.
         */
        //std::thread run(std::unique_ptr<sockpp::tcp_socket> socket) noexcept;

        [[nodiscard]] int send_msg(std::shared_ptr<sockpp::tcp_socket> socket, const std::string_view& msg, const std::string_view& protocol ) const noexcept;


        /**
         * @brief Traite les données reçues du client.
         * @param socket La socket connectée.
         * @return Un entier représentant le résultat du traitement.
         */
        int process(std::shared_ptr<sockpp::tcp_socket> socket);

    private:



        /**
         * @brief Obtient une liste d'adresses IP à envoyer.
         * @return Une chaîne contenant les adresses IP.
         */
        std::string getIpsToSend() const;

        /**
         * @brief Obtient une liste d'images à envoyer.
         * @return Une chaîne contenant les noms des images.
         */
        std::string getPicsToSend() const;

        /**
         * @brief Obtient une image spécifique à envoyer.
         * @param buf_str La chaîne de données reçue.
         * @param buf Le tampon de données reçu.
         * @return Une chaîne contenant l'image à envoyer.
         */
        std::string getPicToSend(const std::string &buf_str, const char *buf) const;

    };

}
