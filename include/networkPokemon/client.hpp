#pragma once
/**
 * @file Client.hpp
 * @brief Définition de la classe Client.
 */


namespace pokemon {

    /**
     * @brief Classe Client pour gérer les connexions.
     */
    class NETWORK_POKEMON_API Client : public NetworkNode {
    public:
        /**
         * @brief Constructeur de la classe Client.
         * @param ip Adresse IP du client.
         * @param port Port du client.
         * @param connector Connecteur TCP partagé.
         */
        Client(std::string_view ip, const in_port_t port, const std::shared_ptr<Node_Info> node_info,
            peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept;
        ~Client() noexcept;

        /**
         * @brief Exécute le client avec une adresse IP voisine et un message.
         * @param nodeToConnectIp Adresse IP du noeud.
         * @param nodeToConnectPort Port du noeud.
         * @param msg Message à envoyer.
         * @return Un thread pour l'exécution.
         */
        std::thread run(std::string_view nodeToConnectIp, const in_port_t nodeToConnectPort, const std::string &msg) noexcept;

        /**
         * @brief permet de gérer la recherche de l'image et pour le télécharger .
         * @param picName Nom du fichier image.
         */
        void getPic(const std::string &picName) noexcept;


        void add_new_node(std::string peer_name, std::string peer_ip, int port) noexcept;
        void get_picture(std::string hash) noexcept;
        void get_client_id(std::string_view ip, in_port_t port) noexcept;
        void get_client_picture(std::string hash) noexcept;

        std::shared_ptr<Image> add_pokemon(std::string_view name, std::string_view picturePath) noexcept;


    private:
        /**
         * @brief Démarrage du client avec une adresse IP voisine et un message.
         * @param neighbour_host Adresse IP du voisin.
         * @param neighbour_port Port du voisin.
         * @param msg Message à envoyer.
         * @return 0 si tout s'est bien passé, -1 sinon.
         */
        int start(std::string_view neighbour_host, const in_port_t neighbour_port, std::string_view msg) noexcept;

        std::string ip_s; ///< Adresse IP du client.

        bool m_running = true;
        std::vector<std::thread> m_threads;
        std::mutex m_thread_mutex;


        /**
         * @brief Exécute la récupération des adresses IP.
         */
        void run_getIp() noexcept;

        /**
         * @brief Obtient les adresses IP des voisins.
         * @return Un thread pour l'exécution.
         */
        [[nodiscard]] std::thread getIps() noexcept;

        /**
         * @brief Ajoute les adresses IP à la liste.
         * @param ips_str Chaîne d'adresses IP.
         * @param id Identifiant.
         */
        void addIps(const std::string &ips_str) const noexcept;

        /**
         * @brief Ajoute les images à la liste.
         * @param str Chaîne d'images.
         */
        void addPictures(const std::string &str) const noexcept;

        /**
         * @brief Ajoute une image à la liste.
         * @param str Chaîne d'image.
         * @param nodeIdStr Identifiant du nœud.
         */
        void addPicture(const std::string &str) const noexcept;

        int check_connected(std::string_view neighbour_ip, in_port_t neighbour_port) noexcept;

        void get_client_ip() noexcept;
        void get_client_pictures() noexcept;
        void check_connected_nodes() noexcept;
    };
}
