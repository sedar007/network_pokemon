#pragma once
/**
 * @file Node.hpp
 * @brief Définition de la classe Node.
 */

namespace pokemon {

    /**
     * @brief Classe pour gérer un nœud.
     */
    class NETWORK_POKEMON_API Node : public Helper {
    public:
        /**
         * @brief Constructeur de la classe Node.
         */
        Node() noexcept;

        static constexpr std::string_view NODE_INFO_FILE = "node_info.json";

        void set_node_info(std::string_view node_name) noexcept;

        inline void setStoragePath(std::string path) noexcept {
            storagePath_s = path;
        }

        [[nodiscard]] inline  Node_Info get_node_info() const noexcept {
            return *m_node_info;
        }


        /**
         * @brief intitialized the node.
         * @param picturePath Chemin vers les images.
         * @param nodeFile Fichier du nœud.
         */
        void initialized(std::string path, const std::string &picturePath, const std::string &nodeFile) noexcept;

        /**
         * @brief Surcharge de l'opérateur de flux de sortie pour afficher le nœud.
         * @param os Flux de sortie.
         * @param node Nœud à afficher.
         * @return Le flux de sortie modifié.
         */
    //    friend std::ostream &operator<<(std::ostream &os, const Node &node);

        /**
         * @brief Obtient l'identifiant du nœud.
         * @return L'identifiant du nœud.
         */
   //     std::string getId() const;

        /**
         * @brief Exécute le nœud.
         */
      //  void run();

        /**
         * @brief Obtient une image à partir d'un nom de fichier.
         * @param picName Nom du fichier image.
         */
        //inline void getPic(const std::string &picName) noexcept { client->getPic(picName);}

    private:
        in_port_t port_s; ///< Port du nœud.
        std::string ip_s; ///< Adresse IP du nœud.
        std::unique_ptr<Node_Info> m_node_info;
        std::string storagePath_s;
      /*  ResourceManager &resourceManager = ResourceManager::getInstance(); ///< Gestionnaire de ressources.
        Trace &trace = Trace::getInstance(); ///< Traceur.
        std::shared_ptr<sockpp::tcp_connector> connector; ///< Connecteur TCP partagé.
        std::unique_ptr<Listen> listen; ///< Écouteur.
        std::unique_ptr<Client> client; ///< Client.
        std::mutex mutex; ///< Mutex pour la synchronisation. */

        /**
         * @brief Ajoute les nœuds à la liste.
         * @param fileName Nom du fichier contenant les nœuds.
         */
      //  void addNodesList(const std::string &fileName);

        /**
         * @brief Ajoute les images à la liste.
         * @param fileName Nom du fichier contenant les images.
         */
       // void addImagesList(const std::string &fileName);

        /**
         * @brief Démarre l'écoute du nœud.
         */
        //void startListening();
    };

}
