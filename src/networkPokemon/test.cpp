#include "pch.h"


namespace pokemon {

    Test::Test() {}

    int Test::getAge() {
        return age;
    }
    void Test::setAge(int _age) {
        this->age = _age;
    }

    std::string pokemon::Test::getLocalIp() {
        sockpp::tcp_connector conn({sockpp::inet_address("8.8.8.8", 53)});
        if (!conn) {
            std::cerr << "Erreur création socket: " << conn.last_error_str() << std::endl;
            return "error";
        }

        // Récupérer l'adresse locale assignée par le système
        auto addr = conn.address();
        return addr.to_string();
    }

}
