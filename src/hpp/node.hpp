/**
 * @file Node.hpp
 * @brief Définition de la classe Node.
 */

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "resourceManager.hpp"
#include "thread"
#include "sockpp/tcp_connector.h"
#include "hpp/listen.hpp"
#include "hpp/client.hpp"

namespace pokemon {

    /**
     * @brief Classe pour gérer un nœud.
     */
    class Node : public Helper {
    public:
        /**
         * @brief Constructeur de la classe Node.
         * @param picturePath Chemin vers les images.
         * @param nodeFile Fichier du nœud.
         */
        Node(const std::string &picturePath, const std::string &nodeFile);

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
        void getPic(const std::string &picName);

    private:
        in_port_t port_s; ///< Port du nœud.
        std::string ip_s; ///< Adresse IP du nœud.
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
        void addNodesList(const std::string &fileName);

        /**
         * @brief Ajoute les images à la liste.
         * @param fileName Nom du fichier contenant les images.
         */
        void addImagesList(const std::string &fileName);

        /**
         * @brief Démarre l'écoute du nœud.
         */
        void startListening();
    };

}
