#pragma once
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


    // Data
    static constexpr int LISTEN_ERROR_CONNECTION_SLEEP_RANGE_BEGIN = 1000;
    static constexpr int LISTEN_ERROR_CONNECTION_SLEEP_RANGE_END = 2000;
    static constexpr size_t SERVER_BUF_SIZE = 10;
    static constexpr size_t FORMATTED_NUMBER_SIZE = 6; ///< nombre de charactere de la taille des messages.

    // Exception
    static std::runtime_error MSG_ERROR_CREATING_ACCEPTOR("Error creating the acceptor");



    static constexpr std::string_view LISTEN = "listen";
    static constexpr std::string_view CLIENT = "client";
    static constexpr std::string_view SERVER = "server";

    // MSG
    static constexpr std::string_view MSG_NODE_ID = "[node - {} ] - {}";
    static constexpr std::string_view MSG_LISTENING_START = "{} - server started listening";
    static constexpr std::string_view MSG_LISTENING_ERROR_CREATING_ACCEPTOR = "{} - Error creating the acceptor";
    static constexpr std::string_view MSG_LISTENING_ERROR_ACCEPTING_ACCEPTOR = "{} - Error accepting incoming connection: {}";
    static constexpr std::string_view MSG_LISTENING_ACCEPTING_CONNECTION = "{} - listening Server started";
    static constexpr std::string_view MSG_LISTENING_AWAITING_CONNECTION = "{} - Awaiting connections";

    static constexpr std::string_view MSG_CLIENT_TRYING_TO_CONNECT = "{} - starting to connect to {}";
    static constexpr std::string_view MSG_CLIENT_ERROR_CONNECTING = "{} - Error connecting to server at: {} on port {}";
    static constexpr std::string_view MSG_CLIENT_CONNECTED = "{} - connected to server at: {} on port {}";
    static constexpr std::string_view MSG_CLIENT_ERROR_WRITING_TCP_STREAM = "{} - Error writing to the TCP stream: {}";
    static constexpr std::string_view MSG_CLIENT_ERROR_READING_TCP_STREAM = "{} - Error reading from the TCP stream: {}";
    static constexpr std::string_view MSG_CLIENT_ERROR_CONVERTING_SIZE = "{} - Error can't convert {} to a integer";
    static constexpr std::string_view MSG_CLIENT_ERROR_INVALID_IP = "{} - Ip Invalid: {}";






    static constexpr std::string_view MSG_SERVER_RECEIVED_CONNECTION = "{} - received connection from {} on port {}";
    static constexpr std::string_view MSG_SERVER_RECEIVED_QUERY = "{} - received < {} > query";
    static constexpr std::string_view MSG_SERVER_SENT_RESPONSE = "{} - response sent to {} on port {}";
    static constexpr std::string_view MSG_SERVER_SENT_IPS_LIST = "{} - sent list of ips to {}";
    static constexpr std::string_view MSG_SERVER_SENT_PICTURES_LIST = "{} - sent list of pictures to {}";
    static constexpr std::string_view MSG_SERVER_SENT_PICTURE = "{} - sent picture to {}";



    static const std::string NO_PICTURE_FOUND(const std::string & pictureName){
        return "L'image <" + pictureName + "> n'a pas été trouvé.\n";
    }

}
