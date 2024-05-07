#pragma once

#include "iostream"
#include "trace.hpp"

namespace pokemon {
    /// @brief Classe pour gérer les ressources, y compris les images et les nœuds.
    class ResourceManager {
    public:
        /// @brief Méthode statique pour obtenir l'instance unique du singleton
        static ResourceManager &getInstance();

        /// @brief Supprimer les méthodes spéciales pour empêcher la copie et l'affectation
        ResourceManager(const ResourceManager &) = delete;

        ResourceManager &operator=(const ResourceManager &) = delete;

        /* ---  Picture  --- */

        /// @brief Ajoute une image à la liste des ressources.
        /// @param pictureName Le nom de l'image.
        /// @param extention L'extension de l'image.
        /// @param pictureHash Le hachage de l'image.
        /// @param owner Le propriétaire de l'image.
        void addPicture(const std::string &pictureName, const std::string &extention, const std::string &pictureHash,
                      const std::string &owner);

        /// @brief Affiche la liste des images.
        /// @param os Flux de sortie où afficher la liste.
        void tracePicturesList(std::ostream &os) const;

        /// @brief Affiche la liste des images de Pokémon.
        /// @param os Flux de sortie où afficher la liste.
        void printPokemonPictures(std::ostream &os) const;

        // <pictureName, {ipOwner, extention, pictureHash} >
        /// @brief Récupère la liste des images.
        /// @return Une map associant le nom de l'image à un tuple contenant l'IP du propriétaire, l'extension et le hachage de l'image.
        std::unordered_map<std::string, std::tuple<std::string, std::string, std::string>> getPicturesList() const;

        // <pictureName, <extention, pictureHash>>
        /// @brief Recherche une image par son nom.
        /// @param name Le nom de l'image à rechercher.
        /// @return Un tuple contenant l'extension et le hachage de l'image s'il est trouvé, sinon std::nullopt.
        std::optional<std::tuple<std::string, std::string, std::string>> findPicture(const std::string &name) const;

        /// @brief Sauvegarde une image sur le disque.
        /// @param location Emplacement où sauvegarder l'image.
        /// @param pictureName le nom de l'image.
        /// @param extension l'extension de l'image.
        /// @param pic_str Données de l'image.
        /// @return 0 si la sauvegarde est réussie, sinon un code d'erreur.
        int savedPictureToDisk(const std::string &location, const std::string &pictureName, std::string &extension, const std::string &pic_str);

        /// @brief Récupère les données de l'image par son hachage.
        /// @param picHash Le hachage de l'image.
        /// @return Les données de l'image.
        std::string getPic_str(const std::string &picHash);

        /// @brief Ajoute un chemin d'accès pour les images.
        /// @param picturePath Le chemin d'accès des images.
        void addPicturePath(const std::string &picturePath);

        /// @brief Récupère le nom et l'extension de l'image par son hachage.
        /// @param imageHash Le hachage de l'image.
        /// @return Un tuple contenant le nom et l'extension de l'image.
        std::tuple<std::string, std::string> getNameAndExtension(const std::string &pictureHash);

        /* ---  Nodes  --- */

        /// @brief Ajoute un nœud à la liste des ressources.
        /// @param ip L'adresse IP du nœud à ajouter.
        void addNode(const std::string &ip);

        /// @brief Récupère la liste des nœuds.
        /// @return Un vecteur contenant les adresses IP des nœuds.
        std::vector<std::string> getNodesList() const;

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
        ResourceManager() { picturePath_s = ""; }

        // Instance unique de resourceManager
        static ResourceManager *resourceManager;
        // Mutex pour la synchronisation
        static std::mutex mutex;
        // <nomImage, {ipOwner, extention, pictureHash} >
        std::unordered_map<std::string, std::tuple<std::string, std::string, std::string>> pictureList_mp; // Liste des pictures
        std::vector<std::string> nodesList_v; // Liste des nœuds.
        Trace &trace = Trace::getInstance();  // Instance unique de trace.
        std::string picturePath_s; // Chemin d'accès des pictures.
    };
}