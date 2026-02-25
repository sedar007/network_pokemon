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
        std::uniform_real_distribution<double> duration_distrib(
            static_cast<double>(first.count()),
            static_cast<double>(seconds.count())
        );
        return std::chrono::seconds(static_cast<int>(duration_distrib(rng_)));
    }


    bool Helper::read_exact(sockpp::tcp_connector& socket, char* buffer, size_t length) {
        size_t total_read = 0;
        while (total_read < length) {
            size_t n = socket.read(buffer + total_read, length - total_read);

            if (n <= 0) {
                return false;
            }
            total_read += n;
        }
        return true;
    }


    size_t Helper::protocolSize() const {
        return protocol_size;
    }

}
