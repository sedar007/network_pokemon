#include "../hpp/main.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include "../hpp/const.hpp"

namespace pokemon {

    Main::Main(const bool traceLog) {
        if (traceLog)
            trace.On();
    }

    Main::Main() {}

    void Main::addNode(std::unique_ptr<Node> &node) {
        node_uptr = std::move(node);
    }

    std::thread Main::run() {
        return std::thread([this] { programme(); });
    }

    std::string Main::toLower(std::string s) {
        // Convertir en miniscule
        std::transform(s.begin(), s.end(), s.begin(),  [](unsigned char c)  {
                return std::tolower(c);
        });
        return s;
    }

    std::string& Main::ltrim(std::string &str, std::string const &space = " \r\n\t\v\f"){
        str.erase(0, str.find_first_not_of(space));
        return str;
    }

    std::string& Main::rtrim(std::string &str, std::string const &space = " \r\n\t\v\f"){
        str.erase(str.find_last_not_of(space) + 1);
        return str;
    }

    std::string& Main::trim(std::string &str, std::string const &space=" \r\n\t\v\f"){
        return ltrim(rtrim(str, space), space);
    }

    void Main::programme() {
        if (node_uptr == nullptr){
            std::cout<<ERROR_STARTING_PROGRAM<<std::endl;
            trace.print(std::cerr, "Error: node is null");
            return;
        }

        std::string menu;


        std::cout << MENU_HEADER<< std::endl;
        do {
            std::cout << MENU_OPTIONS << std::endl;
            std::cout << MENU_OPTION_1 << std::endl;
            std::cout << MENU_OPTION_2 << std::endl;
            std::cout << MENU_OPTION_3 << std::endl;
            std::cout << MENU_OPTION_4 << std::endl;
            std::cout <<MENU_CHOICE_PROMPT;

            getline(std::cin, menu);

            if (menu == "1")
                resourceManager.printNodesList(std::cout);
            else if (menu == "2")
                resourceManager.printPokemonPictures(std::cout);
            else if (menu == "3") {
                std::cout << ENTER_PICTURE_NAME;
                std::string picName;
                getline(std::cin, picName);
                node_uptr->getPic(toLower(trim(picName)));
            }
            else if(menu == "4" || toLower(trim(menu)) == EXIT){
                std::cout << GOODBYE_MESSAGE<< std::endl;
                break;
            }
            else{
                std::cout << INVALID_COMMAND_MESSAGE << std::endl;
            }
            std::cout<<std::endl;
        } while (true);
    }
}