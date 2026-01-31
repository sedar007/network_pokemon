#pragma once

/**
 * @file listen.hpp
 * @brief Définition de la classe Listen.
 */


namespace pokemon {

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

    private:

    };
}
