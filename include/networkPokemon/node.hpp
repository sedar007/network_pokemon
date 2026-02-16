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

        static constexpr std::string_view NODE_INFO_FILE = "node_infos.json";
        static constexpr std::string_view NODE_LIST_FILE = "node_list.json";
        static constexpr std::string_view IMAGE_LIST_FILE = "pokemons_images.json";

        static constexpr std::string_view EN0_INTERFACE = "en0";
        static constexpr std::string_view LOCALHOST_IP = "127.0.0.1";
        static constexpr std::string_view Lo_0_INTERFACE = "lo0";
        static constexpr int DEFAULT_PREFERRED_PORT = 49153;

        void set_node_info(std::string_view node_name) noexcept;

        inline void setStoragePath(std::string path) noexcept {
            storagePath_s = path;
        }

        [[nodiscard]] inline  Node_Info get_node_info() const noexcept {
            return *m_node_info;
        }

        void add_new_peer(std::string peer_name, std::string peer_port) noexcept;
        void add_peer(std::string id, std::string peer_name, std::string peer_ip, int port) noexcept;

        void addNodesList(const std::string &fileName);

        [[nodiscard]] std::vector<Image> get_image_list() const noexcept {
            return resourceManager.getImagesList();
        }

        void add_pokemon(std::string_view name, std::string_view picturePath) noexcept;
        void remove_pokemon(std::string_view name, std::string_view picturePath) noexcept;


        std::string get_picture(const Image image);

        /**
         * @brief intitialized the node.
         * @param path the path to the node configuration and data files.
         */
        void initialized(std::string_view path) noexcept;

        /**
         * @brief Surcharge de l'opérateur de flux de sortie pour afficher le nœud.
         * @param os Flux de sortie.
         * @param node Nœud à afficher.
         * @return Le flux de sortie modifié.
         */
        friend std::ostream &operator<<(std::ostream &os, const Node &node);

        /**
         * @brief Obtient l'identifiant du nœud.
         * @return L'identifiant du nœud.
         */
        std::string getId() const;

        /**
         * @brief Exécute le nœud.
         */
        void run();

        /**
         * @brief Obtient une image à partir d'un nom de fichier.
         * @param picName Nom du fichier image.
         */
        inline void getPic(const std::string &picName) noexcept { client->getPic(picName);}


        /**
         * @brief Obtient l'adresse IP du nœud.
         * @return L'adresse IP du nœud.
         */
        [[nodiscard]] std::string get_ip() const noexcept {
            return m_node_info->get_ip().data();
        }

        [[nodiscard]] std::vector<Node_Info> get_node_list() const noexcept {
            return resourceManager.getNodesInfoList();
        }


    private:
        in_port_t port_s; ///< Port du nœud.
        std::string ip_s; ///< Adresse IP du nœud.
        std::shared_ptr<Node_Info> m_node_info;
        std::string storagePath_s;
        ResourceManager &resourceManager = ResourceManager::getInstance(); ///< Gestionnaire de ressources.
        Trace &trace = Trace::getInstance(); ///< Traceur.
        std::shared_ptr<sockpp::tcp_connector> connector; ///< Connecteur TCP partagé.
        std::unique_ptr<Listen> listen; ///< Écouteur.
        std::unique_ptr<Client> client; ///< Client.
        std::mutex mutex; ///< Mutex pour la synchronisation.

        /**
         * @brief Ajoute les nœuds à la liste.
         * @param fileName Nom du fichier contenant les nœuds.
         */
        void addNodesList();


        void addImagesList();

        /**
         * @brief Ajoute les images à la liste.
         * @param fileName Nom du fichier contenant les images.
         */
        void addImagesList(const std::string &fileName);

        /**
         * @brief Démarre l'écoute du nœud.
         */
        void startListening();

        /**
         *@brief Get the node ip
         *@return std::string
         */
        std::string get_network_ip() const;
        int find_available_port(int preferred_port);
        bool is_node_online(std::string_view ip) const noexcept;
    };

}
