#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API image_repository {
    public:

        inline static constexpr std::string_view BASE64_CHARS =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        inline static constexpr std::array<std::string_view, 5> ALLOWED_IMAGE_EXTENSIONS = {
            ".png", ".jpg", ".jpeg", ".gif", ".bmp"
        };
        static constexpr uintmax_t MAX_IMAGE_BYTES = 64ull * 1024 * 1024;

        explicit image_repository(std::string_view storagePath);

        void add_image(const Image& image) noexcept;
        void add_image(std::shared_ptr<Image> image) noexcept;
        void remove_image(std::string_view hash) noexcept;
        std::optional<Image> find_image(std::string_view hash) const noexcept;
        bool save_image_to_disk(const std::string& filename, const std::string& data) const;
        std::shared_ptr<Image> save_image(std::string_view name, std::string_view owner_id, std::filesystem::path image_to_save_path) noexcept;

        inline std::vector<Image> get_images() const noexcept {
             std::shared_lock lock(mutex_);
             return images_;
        }

        std::shared_ptr<Image> add_picture_from_path(std::string_view name, std::string_view owner_id, std::string_view picturePath) noexcept;

        [[nodiscard]] inline std::string_view get_storage_path() const noexcept {
            return storagePath_;
        }



        std::string get_picture_base64(const Image image);

        // Recomputes the SHA-256 of the decoded payload and compares it against
        // the hash announced by the sender, so a truncated/corrupted/tampered
        // transfer is rejected instead of silently cached.
        [[nodiscard]] static bool verify_base64_sha256(std::string_view hash, std::string_view base64_data) noexcept;

    private:
        std::string storagePath_;
        mutable std::shared_mutex mutex_;
        std::vector<Image> images_;

        static std::string base64_encode(const std::string &in) noexcept;
        static std::string base64_decode(std::string_view in) noexcept;
        std::string getPic_str(const Image image);
        static std::string calculate_sha256(const std::string& data);
        [[nodiscard]] static bool is_allowed_image_extension(std::string_view extension) noexcept;
    };

}

