/**
 * @file listen.hpp
 * @brief Définition de la classe Listen.
 */

#pragma once

#include "iostream"
#include <thread>
#include "sockpp/tcp_socket.h"
#include "resourceManager.hpp"
#include "server.hpp"
#include "trace.hpp"

namespace pokemon {

    /**
     * @brief Classe responsable de l'écoute des connexions sur un port spécifique.
     */
    class Listen : public Helper{
    public:
        /**
         * @brief Constructeur de la classe Listen.
         * @param port Port sur lequel écouter les connexions.
         */
        Listen(const in_port_t port);

        /**
         * @brief Lance l'écoute des connexions sur un port spécifique dans un thread séparé.
         * @return Un objet std::thread représentant le thread d'écoute.
         */
        std::thread connect();

    private:
        /**
         * @brief Méthode interne pour écouter les connexions entrantes.
         * @return 1 en cas d'erreur, sinon 0.
         */
        int listening();

        in_port_t port; ///< Port sur lequel écouter les connexions.
        ResourceManager &resourceManager = ResourceManager::getInstance(); ///< Singleton pour gerer les ressoures.
        Trace &trace = Trace::getInstance();///< Singleton pour gerer les traces (logs ... ).
    };
}