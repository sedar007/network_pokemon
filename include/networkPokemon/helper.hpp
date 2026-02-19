/**
 * @file helper.hpp
 * @brief Définition de la classe Helper et de l'énumération PROTOCOLE.
 */
#pragma once

namespace pokemon {
    enum class PROTOCOL {
        GET_IPS, GET_PICS, GET_PIC, GET_ALIVE, GET_ID, UNKNOWN
    };

    /**
     * @brief Classe utilitaire contenant des méthodes pour manipuler les protocoles et les adresses IP.
     */
    class NETWORK_POKEMON_API Helper {
        /**
         * @brief Vérifie si une chaîne de caractères représente une adresse IP valide au format "XXX.XXX.XXX.XXX:port".
         * @param str Chaîne de caractères à vérifier.
         * @return true si la chaîne est une adresse IP valide, sinon false.
         */

    public:
        /**
         * @brief Constructeur par défaut de la classe Helper.
         */
        Helper() noexcept;
        [[nodiscard]] static bool isValidIPAddressPort(const std::string &str);
        [[nodiscard]] static bool isValidIPAddress(const std::string &str);



        bool read_exact(sockpp::tcp_connector& socket, char* buffer, size_t length);

        /**
         * @brief Génère un délai aléatoire entre a et b.
         * @param a Valeur minimale du délai.
         * @param b Valeur maximale du délai.
         * @return Durée aléatoire entre a et b.
         */
        std::chrono::milliseconds threadSleep_s(unsigned int a, unsigned int b);
        std::chrono::seconds threadSleep_seconde(std::chrono::seconds first, std::chrono::seconds seconds);

        /**
         * @brief Génère une chaîne de caractères représentant un nombre formaté selon un protocole spécifique.
         * @param number Le nombre à formater.
         * @return Chaîne de caractères représentant le nombre formaté selon le protocole.
         */
        [[nodiscard]] inline std::string generateFormattedNumber(const size_t number) const noexcept {
            std::ostringstream oss;
            oss << std::setw(FORMATTED_NUMBER_SIZE) << std::setfill('0') << number;
            return oss.str();
        }

        /**
         * @brief Retourne une chaîne de caractères représentant un protocole donné.
         * @param q Le protocole à convertir en chaîne de caractères.
         * @return Chaîne de caractères représentant le protocole.
         */
        std::string protocolToString(const PROTOCOL q) const;
        PROTOCOL string_to_protocol(std::string_view s) const;


        /**
         * @brief Retourne le nombre de charactere de la taille des requetes.
         * @return Taille du charactere.
         */
        size_t protocolSize() const;


    public:
        std::random_device rd_; ///< Générateur de nombres aléatoires.
        std::mt19937 rng_{rd_()}; ///< Générateur de nombres aléatoires.
        size_t protocol_size; ///< nombre de charactere de la taille des requetes.


    };

} // namespace pokemon
