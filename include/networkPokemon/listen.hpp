#pragma once
/**
 * @file listen.hpp
 * @brief Définition de la classe Listen.
 */


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
        Listen(const in_port_t port) noexcept;

        /**
         * @brief Lance l'écoute des connexions sur un port spécifique dans un thread séparé.
         * @return Un objet std::thread représentant le thread d'écoute.
         */
        [[nodiscard]] std::thread connect();

    private:
        /**
         * @brief Méthode interne pour écouter les connexions entrantes.
         *
         */
        void listening();

        in_port_t port; ///< Port sur lequel écouter les connexions.
        ResourceManager &resourceManager = ResourceManager::getInstance(); ///< Singleton pour gerer les ressoures.
        Trace &trace = Trace::getInstance();///< Singleton pour gerer les traces (logs ... ).
    };
}
