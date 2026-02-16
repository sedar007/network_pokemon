#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>   // Nécessaire pour fcntl
#include <string>
#include <iostream>

#include "pch.h"

namespace pokemon {
    image_cache::image_cache(std::string_view hash, std::string_view data ) noexcept
        : hash_s(hash)
        , data_s(data)
        {}
}
