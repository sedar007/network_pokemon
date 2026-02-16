#pragma once
#include <vector>

namespace pokemon {
    /// @brief Classe pour gérer les ressources, y compris les images et les nœuds.
    class NETWORK_POKEMON_API ResourceManager {
    public:
        /// @brief Méthode statique pour obtenir l'instance unique du singleton
        static ResourceManager &getInstance();

        /// @brief Supprimer les méthodes spéciales pour empêcher la copie et l'affectation
        ResourceManager(const ResourceManager &) = delete;

        ResourceManager &operator=(const ResourceManager &) = delete;

        inline void set_path(std::string_view path) noexcept {
            path_s = path;
        }

        [[nodiscard]] inline std::string get_path() const noexcept {
            return path_s;
        }

        /* ---  Picture  --- */


        /// @brief Affiche la liste des images.
        /// @param os Flux de sortie où afficher la liste.
        void tracePicturesList(std::ostream &os) const;

        /// @brief Affiche la liste des images de Pokémon.
        /// @param os Flux de sortie où afficher la liste.
        void printPokemonPictures(std::ostream &os) const;

        /// @brief Sauvegarde une image sur le disque.
        /// @param location Emplacement où sauvegarder l'image.
        /// @param pictureName le nom de l'image.
        /// @param extension l'extension de l'image.
        /// @param pic_str Données de l'image.
        /// @return 0 si la sauvegarde est réussie, sinon un code d'erreur.
        int savedPictureToDisk(const std::string &location, const std::string &pictureName, std::string &extension, const std::string &pic_str);


        /// @brief Récupère le nom et l'extension de l'image par son hachage.
        /// @param imageHash Le hachage de l'image.
        /// @return Un tuple contenant le nom et l'extension de l'image.
        std::tuple<std::string, std::string> getNameAndExtension(const std::string &pictureHash);

        /* ---  Nodes  --- */


        /// @brief Affiche la liste des nœuds.
        /// @param os Flux de sortie où afficher la liste.
        void printNodesList(std::ostream &os) const;

        /// @brief Recherche un nœud par son adresse IP.
        /// @param ip L'adresse IP du nœud à rechercher.
        /// @return L'adresse IP du nœud s'il est trouvé, sinon std::nullopt.
        std::optional<std::string> findNode(const std::string &ip) const;


        /// @brief Vérifie si un conteneur est vide.
        /// @tparam T Type du conteneur.
        /// @param t Le conteneur à vérifier.
        /// @return true si le conteneur est vide, sinon false.
        template<typename T>
        bool empty(const T &t) const {
            return t.empty();
        }

    private:
        // Constructeur privé pour empêcher l'instanciation en dehors de la classe
        ResourceManager() { }
        // Mutex pour la synchronisation
        static std::mutex mutex;
        Trace &trace = Trace::getInstance();  // Instance unique de trace.
        std::string path_s;
    };
}
