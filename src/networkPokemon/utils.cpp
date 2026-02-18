#include "pch.h"

namespace pokemon {

    std::string Utils::generate_uuid_v4() noexcept {
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

    std::string Utils::formatted_number(size_t number) noexcept {
        return std::format("{:0{}}", number, FORMATTED_NUMBER_SIZE);
    }


    bool Utils::read_exact(std::shared_ptr<sockpp::tcp_connector> connector, char* buffer, size_t length) noexcept {
        size_t total_read = 0;
        while (total_read < length) {
            ssize_t n = connector->read(buffer + total_read, length - total_read);
            if (n <= 0) return false;
            total_read += n;
        }
        return true;
    };


    size_t Utils::get_total_bytes_from_connector(const std::shared_ptr<sockpp::tcp_connector> &connector) {
        char sizeHeader[FORMATTED_NUMBER_SIZE];

        if (!Utils::read_exact(connector, sizeHeader, FORMATTED_NUMBER_SIZE)) {
            connector->shutdown(SHUT_RDWR);
            throw std::runtime_error("Failed to read size header");
        }

        try {
            return  std::stoul(std::string(sizeHeader, FORMATTED_NUMBER_SIZE));

        } catch(...) {
            connector->shutdown(SHUT_RDWR);
            throw std::runtime_error("Failed to convert size header to number");
        }
    }


    std::string Utils::safe_string(const char* data, size_t max_len) noexcept {
        size_t len = 0;
        while(len < max_len && data[len] != '\0') len++;
        return std::string(data, len);
    }






}
