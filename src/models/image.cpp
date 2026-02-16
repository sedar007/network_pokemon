#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>   // Nécessaire pour fcntl
#include <string>
#include <iostream>

#include "pch.h"

namespace pokemon {
    Image::Image(std::string_view name, std::string_view extension, std::string_view hash,
        std::string_view owner, std::string_view size, std::string_view size_unit ) noexcept
        : name_s(name)
        , extension_s(extension)
        , hash_s(hash)
        , owner_s(owner)
        , size_s(size)
        , size_unit_s(size_unit){
    }

}
