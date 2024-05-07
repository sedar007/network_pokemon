/**
 * @file Main.hpp
 * @brief Définition de la classe Main.
 */

#pragma once

#include <thread>
#include <string>
#include <memory>
#include "node.hpp"
#include "resourceManager.hpp"
#include "trace.hpp"

namespace pokemon {

    /**
     * @brief Classe principale pour gérer les nœuds.
     */
    class Main {
    public:
        /**
         * @brief Constructeur par défaut de la classe Main.
         */
        Main();

        /**
         * @brief Constructeur de la classe Main avec option de log.
         * @param traceLog Indique si log est activée.
         */
        Main(const bool traceLog);

        /**
         * @brief Ajoute un nœud au programme.
         * @param node Pointeur unique vers le nœud à ajouter.
         */
        void addNode(std::unique_ptr<Node> &node);

        /**
         * @brief Exécute le programme principal.
         * @return Un thread pour l'exécution.
         */
        std::thread run();

    private:
        std::unique_ptr<Node> node_uptr; ///< Pointeur unique vers le nœud.
        ResourceManager &resourceManager = ResourceManager::getInstance(); ///< Gestionnaire de ressources.
        Trace &trace = Trace::getInstance(); ///< Traceur.

        /**
         * @brief Convertit une chaîne en minuscules.
         * @param s Chaîne à convertir.
         * @return La chaîne convertie en minuscules.
         */
        std::string toLower(std::string s);

        /**
         * @brief Supprime les espaces à gauche d'une chaîne.
         * @param str Chaîne à modifier.
         * @param space Espaces à supprimer.
         * @return La chaîne modifiée.
         */
        std::string& ltrim(std::string &str, std::string const &space);

        /**
         * @brief Supprime les espaces à droite d'une chaîne.
         * @param str Chaîne à modifier.
         * @param space Espaces à supprimer.
         * @return La chaîne modifiée.
         */
        std::string& rtrim(std::string &str, std::string const &space);

        /**
         * @brief Supprime les espaces à gauche et à droite d'une chaîne.
         * @param str Chaîne à modifier.
         * @param space Espaces à supprimer.
         * @return La chaîne modifiée.
         */
        std::string& trim(std::string &str, std::string const &space);

        /**
         * @brief Exécute le programme principal.
         */
        void programme();
    };

}
