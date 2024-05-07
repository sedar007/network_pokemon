#pragma once

#include "thread"
#include "sockpp/tcp_connector.h"
#include "resourceManager.hpp"
#include "trace.hpp"
#include "random"
#include "helper.hpp"

namespace pokemon {
    static const std::string SAVE_PICTURE_FAIL = "Une erreur s'est produite, l'image n'a pas été téléchargée, réessaye";
    static const std::string FOLDER_TO_SAVE = "../Téléchargements/";
    static const std::string SAVE_PICTURE_SUCCESS = "L'image a été téléchargée avec succès";
    static const std::string ENTER_PICTURE_NAME = "Entrer le nom de l'image: ";

    static const std::string MENU_HEADER = " ----- PokeGallery -----";
    static const std::string MENU_OPTIONS = "** Menus **";
    static const std::string MENU_OPTION_1 = "1. Affiche la liste des serveurs connus par le noeud.";
    static const std::string MENU_OPTION_2 = "2. Affiche la liste des images de Pokemon disponibles sur le noeud (une liste de noms et de hash code)";
    static const std::string MENU_OPTION_3 = "3. Demande une image particulière";
    static const std::string MENU_OPTION_4 = "4. Exit, pour quitter";
    static const std::string MENU_CHOICE_PROMPT = "Choix (1, 2, 3, exit): ";
    static const std::string GOODBYE_MESSAGE = "Au revoir";
    static const std::string INVALID_COMMAND_MESSAGE = "Commande invalide, Réessayez.";
    static const std::string EXIT = "exit";
    static const std::string ERROR_STARTING_PROGRAM = "Quelque chose s'est mal passée, le programme s'est arreté";



    static const std::string NO_PICTURE_FOUND(const std::string & pictureName){
        return "L'image <" + pictureName + "> n'a pas été trouvé.\n";
    }

}