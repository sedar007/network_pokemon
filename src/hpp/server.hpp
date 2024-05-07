#pragma once

#include <thread>
#include <sockpp/tcp_connector.h>
#include "resourceManager.hpp"
#include "trace.hpp"
#include "helper.hpp"

namespace pokemon {

    /**
     * @brief Représente un serveur pour gérer les connexions entrantes.
     */
    class Server : public Helper {
    public:
        /**
         * @brief Constructeur pour initialiser le serveur avec le port spécifié.
         * @param port Le numéro de port sur lequel écouter.
         */
        Server(in_port_t port);

        /**
         * @brief Démarre le serveur et gère les connexions entrantes.
         * @param socket La socket connectée.
         * @return Un thread qui exécute le serveur.
         */
        std::thread run(sockpp::tcp_socket &socket);

    private:
        /**
         * @brief Traite les données reçues du client.
         * @param socket La socket connectée.
         * @return Un entier représentant le résultat du traitement.
         */
        int process(sockpp::tcp_socket &socket);

        ResourceManager &resourceManager = ResourceManager::getInstance();
        Trace &trace = Trace::getInstance();
        in_port_t port_i;

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
