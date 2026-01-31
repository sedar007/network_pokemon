#pragma once

/**
 * @file listen.hpp
 * @brief Définition de la classe Listen.
 */

#include <fstream>

// Pour alléger le code
namespace pokemon {

    struct Config {
        std::string nodeName;
        int port;
        int maxConnections;
        bool autoShare;
        bool autoDownload;
    };

    // Macro JSON automatique
   // NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, nodeName, port, maxConnections, autoShare, autoDownload)

    /**
     * @brief Classe responsable de l'écoute des connexions sur un port spécifique.
     */
    class NETWORK_POKEMON_API Test {
    public:
        /**
         * @brief Constructeur de la classe Listen.
         * @param port Port sur lequel écouter les connexions.
         */
        Test() noexcept;

        std::string getIp() const noexcept;
        std::string storagePath;
        Config currentConfig;

        void setStoragePath(const std::string& path) {
            storagePath = path;
        }

        // 2. Méthode pour sauvegarder des données
        void savePeerData(const std::string& data) {
            if (storagePath.empty()) return; // Sécurité

            // On construit le chemin complet : path/peers.txt
            std::string fullPath = storagePath + "/peers.txt";

            std::ofstream outfile(fullPath, std::ios::app); // Mode ajout
            if (outfile.is_open()) {
                outfile << data << "\n";
                outfile.close();
            }
        }

        void saveConfig(std::string name, int port, int maxConn, bool share, bool download);
        std::optional<Config> getConfig();


    private:

    };
}
