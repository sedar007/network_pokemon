#pragma once
#include <string_view>

namespace pokemon {

    class NETWORK_POKEMON_MODELS_API Image {
        public:
            Image(std::string_view name, std::string_view extension, std::string_view hash,
                std::string_view owner, std::string_view size, std::string_view size_unit) noexcept;
            Image() noexcept = default;

            static constexpr std::string_view IMAGE_NAME_KEY = "name";
            static constexpr std::string_view IMAGE_EXTENSION_KEY = "extension";
            static constexpr std::string_view IMAGE_HASH_KEY = "hash";
            static constexpr std::string_view IMAGE_OWNER_KEY = "owner";
            static constexpr std::string_view IMAGE_SIZE_KEY = "size";
            static constexpr std::string_view IMAGE_SIZE_UNIT_KEY = "size_unit";

            [[nodiscard]] inline std::string_view get_name() const noexcept {
                return name_s;
            }

            [[nodiscard]] inline std::string_view get_extension() const noexcept {
                return extension_s;
            }

            [[nodiscard]] inline std::string_view get_hash() const noexcept {
                return hash_s;
            }

        inline void set_name(std::string_view name) noexcept {
            name_s = name;
        }
        inline void set_extension(std::string_view extension) noexcept {
            extension_s = extension;
        }

        inline void set_hash(std::string_view hash) noexcept {
            hash_s = hash;
        }

        inline void set_owner(std::string_view owner) noexcept {
            owner_s = owner;
        }

        [[nodiscard]] inline std::string_view get_owner() const noexcept {
            return owner_s;
        }

        inline void set_size(std::string_view size) noexcept {
            size_s = size;
        }

        [[nodiscard]] inline std::string_view get_size() const noexcept {
            return size_s;
        }

        inline void set_size_unit(std::string_view size_unit) noexcept {
            size_unit_s = size_unit;
        }

        [[nodiscard]] inline std::string_view get_size_unit() const noexcept {
            return size_unit_s;
        }




        friend std::ostream &operator<<(std::ostream &os, const Image &image) {
               os << "Image Information:\n"
                  << "  Name: " << image.get_name() << "\n"
                  << "  Extension: " << image.get_extension() << "\n"
                  << "  Hash: " << image.get_hash() << "\n";

                return os;
        }

        friend bool operator==(const Image &lhs, const Image &rhs) noexcept {
            return lhs.get_name() == rhs.get_name() &&
                   lhs.get_extension() == rhs.get_extension() &&
                   lhs.get_hash() == rhs.get_hash();
            }

    private:
        std::string name_s;
        std::string extension_s;
        std::string hash_s;
        std::string owner_s;
        std::string size_s;
        std::string size_unit_s;

    };
}
