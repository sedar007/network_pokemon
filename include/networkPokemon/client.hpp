#pragma once
/**
 * @file Client.hpp
 * @brief Définition de la classe Client.
 */


namespace pokemon {

    /**
     * @brief Classe Client pour gérer les connexions.
     */
    class Client : public Helper {
    public:
        /**
         * @brief Constructeur de la classe Client.
         * @param ip Adresse IP du client.
         * @param port Port du client.
         * @param connector Connecteur TCP partagé.
         */
        Client(const std::string &ip, const in_port_t port) noexcept;

        /**
         * @brief Exécute le client avec une adresse IP voisine et un message.
         * @param nodeToConnectIp Adresse IP du noeud.
         * @param nodeToConnectPort Port du noeud.
         * @param msg Message à envoyer.
         * @return Un thread pour l'exécution.
         */
        std::thread run(const std::string &nodeToConnectIp, const in_port_t nodeToConnectPort, const std::string &msg) noexcept;

        /**
         * @brief permet de gérer la recherche de l'image et pour le télécharger .
         * @param picName Nom du fichier image.
         */
        void getPic(const std::string &picName) noexcept;

    private:
        /**
         * @brief Démarrage du client avec une adresse IP voisine et un message.
         * @param neighbour_host Adresse IP du voisin.
         * @param neighbour_port Port du voisin.
         * @param msg Message à envoyer.
         * @return 0 si tout s'est bien passé, -1 sinon.
         */
        int start(const std::string &neighbour_host, const in_port_t neighbour_port, const std::string &msg) noexcept;

        in_port_t port_s; ///< Port du client.
        std::string ip_s; ///< Adresse IP du client.

        ResourceManager &resourceManager = ResourceManager::getInstance(); ///< Gestionnaire de ressources.
        Trace &trace = Trace::getInstance(); ///< Traceur.

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
    };
}
