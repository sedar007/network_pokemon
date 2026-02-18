#include "pch.h"

namespace pokemon {

     std::string Model::safe_string(const char* data, size_t max_len) noexcept {
        size_t len = 0;
        while(len < max_len && data[len] != '\0') len++;
        return std::string(data, len);
    }
}
