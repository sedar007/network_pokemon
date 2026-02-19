#include "pch.h"

namespace pokemon {
    image_cache::image_cache(std::string_view hash, std::string_view data ) noexcept
        : hash_s(hash)
        , data_s(data)
        {}
}
