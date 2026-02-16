#include "pch.h"

using namespace std::chrono_literals;
namespace pokemon {

    Helper::Helper() noexcept :protocol_size(8) {}

    bool Helper::isValidIPAddressPort(const std::string &str)  {
        // Expression régulière pour valider le format de l'adresse IP
        std::regex pattern("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}:\\d+$");
        return std::regex_match(str, pattern);
    }
    bool Helper::isValidIPAddress(const std::string &str)  {
        std::regex pattern("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}");
        return std::regex_match(str, pattern);
    }

    std::chrono::milliseconds Helper::threadSleep_s(unsigned int a, unsigned int b) {
        if (a > b) {
            auto tmp = a;
            a = b;
            b = tmp;
        }
        std::uniform_real_distribution<double> duration_distrib(a, b);
        return std::chrono::milliseconds(static_cast<int>(duration_distrib(rng_)));
    }

    std::chrono::seconds Helper::threadSleep_seconde(std::chrono::seconds first, std::chrono::seconds seconds) {
        if (first > seconds) {
            auto tmp = first;
            first = seconds;
            seconds = tmp;
        }
        std::uniform_real_distribution<double> duration_distrib(first.count(), seconds.count());
        return std::chrono::seconds(static_cast<int>(duration_distrib(rng_)));
    }


    std::string Helper::protocolToString(const PROTOCOL q) const {
        switch (q) {
            case PROTOCOL::GET_IPS:
                return "GET_IPS_";
            case PROTOCOL::GET_PICS:
                return "GET_PICS";
            case PROTOCOL::GET_PIC:
                return "GET_PIC_";
            case PROTOCOL::GET_ALIVE:
                return "GETALIVE";
            case PROTOCOL::GET_ID:
                return "GET_ID__";
            default:
                return "UNKNOWN_";
        }
    }

    PROTOCOL Helper::string_to_protocol(std::string_view s) const {
        if (s == "GETALIVE")
            return PROTOCOL::GET_ALIVE;
        if (s == "GET_ID__")
            return PROTOCOL::GET_ID;

        return PROTOCOL::GET_IPS; // Valeur par défaut, à adapter selon les besoins
    }


    bool Helper::read_exact(sockpp::tcp_connector& socket, char* buffer, size_t length) {
        size_t total_read = 0;
        while (total_read < length) {
            ssize_t n = socket.read(buffer + total_read, length - total_read);

            if (n <= 0) {
                return false;
            }
            total_read += n;
        }
        return true;
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

    size_t Helper::protocolSize() const {
        return protocol_size;
    }

    std::string Helper::generate_uuid_v4() noexcept {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 15);
        static std::uniform_int_distribution<> dis2(8, 11);

        std::stringstream ss;
        ss << std::hex;

        int i;
        for (i = 0; i < 8; i++) ss << dis(gen);
        ss << "-";
        for (i = 0; i < 4; i++) ss << dis(gen);
        ss << "-4"; // UUID version 4
        for (i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        ss << dis2(gen); // Variant (8, 9, a, b)
        for (i = 0; i < 3; i++) ss << dis(gen);
        ss << "-";
        for (i = 0; i < 12; i++) ss << dis(gen);

        return ss.str();
    }

    std::string Helper::base64_encode(const std::string &in) noexcept {
        std::string out;
        int val = 0, valb = -6;
        for (unsigned char c : in) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                out.push_back(BASE64_CHARS[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6) out.push_back(BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
        while (out.size() % 4) out.push_back('=');
        return out;
    }


}
