#include "../hpp/helper.hpp"
#include <iostream>
#include "chrono"
#include "exception"
#include "regex"

using namespace std::chrono_literals;
namespace pokemon {

    Helper::Helper() :protocol_size(8), formattedNumberSize(6) {}

    bool Helper::isValidIPAddress(const std::string &str)  {
        // Expression régulière pour valider le format de l'adresse IP
        std::regex pattern("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}:\\d+$");
        return std::regex_match(str, pattern);
    }

    std::chrono::milliseconds Helper::threadSleep_s(unsigned int a, unsigned int b) {
        if (a > b) {
            auto tmp = a;
            a = b;
            b = tmp;
        }
        std::uniform_real_distribution<int> duration_distrib(a, b);
        return std::chrono::milliseconds(duration_distrib(rng_));
    }

    std::string Helper::generateFormattedNumber(const int number) const {
        std::ostringstream oss;
        oss << std::setw(formattedNumberSize) << std::setfill('0') << number;
        return oss.str();
    }

    std::string Helper::protocolToString(const PROTOCOLE q) const {
        switch (q) {
            case PROTOCOLE::GET_IPS:
                return "GET_IPS_";
            case PROTOCOLE::GET_PICS:
                return "GET_PICS";
            case PROTOCOLE::GET_PIC:
                return "GET_PIC_";
            default:
                return "UNKNOWN_";
        }
    }

    int Helper::getPort_Ip(const std::string &str, std::string &ip, in_port_t &port) {
        char separateur = ':';
        std::string portString;
        std::stringstream ss(str);
        std::getline(ss, ip, separateur); // Lire l'adresse IP
        std::getline(ss, portString, separateur); // Lire le port
        try {
            port = std::stoi(portString);
            return 0;
        }
        catch (std::exception) {
            return -1;
        }
    }

    std::string Helper::getAdress(const std::string &ip, const in_port_t port) const {
        return ip + ":" + std::to_string(port);
    }

    size_t Helper::getFormattedNumberSize() const {
        return formattedNumberSize;
    }

    size_t Helper::protocolSize() const {
        return protocol_size;
    }


}