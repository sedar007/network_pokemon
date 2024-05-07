#pragma once

#include <iostream>
#include <map>

namespace pokemon {
    /**
     * @brief Classe pour gérer les traces (logs) dans l'application.
     */
    class Trace {
    public:
        /**
         * @brief Obtient l'instance unique du singleton.
         * @return Référence vers l'instance de Trace.
         */
        static Trace &getInstance();

        /**
         * @brief Supprime les méthodes spéciales pour empêcher la copie et l'affectation.
         */
        Trace(const Trace &) = delete;
        Trace &operator=(const Trace &) = delete;

        /**
         * @brief Active les traces.
         */
        void On();

        /**
         * @brief Affiche un message dans les traces.
         * @param os Flux de sortie (par exemple, std::cout).
         * @param msg Message à afficher.
         */
        void print(std::ostream &os, const std::string &msg);

        /**
         * @brief Affiche un message avec un code ANSI dans les traces.
         * @param os Flux de sortie (par exemple, std::cout).
         * @param codeAnsi Code ANSI pour la mise en forme (couleur, style, etc.).
         * @param msg Message à afficher.
         */
        void print(std::ostream &os, const std::string &codeAnsi, const std::string &msg);

    private:
        /**
         * @brief Constructeur privé pour empêcher l'instanciation en dehors de la classe.
         */
        Trace() {}

        // Instance unique de Trace
        static Trace *trace;
        // Mutex pour la synchronisation
        static std::mutex mutex;
        bool affiche = false;
    };
}
