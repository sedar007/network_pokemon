#pragma once

namespace pokemon {

    class NETWORK_POKEMON_API image_repository {
    public:

        inline static constexpr std::string_view BASE64_CHARS =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";

        explicit image_repository(std::string_view storagePath);

        void add_image(const Image& image) noexcept;
        void add_image(std::shared_ptr<Image> image) noexcept;
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



    private:
        std::string storagePath_;
        mutable std::shared_mutex mutex_;
        std::vector<Image> images_;

        std::string base64_encode(const std::string &in) noexcept;
        std::string getPic_str(const Image image);
        std::string calculate_sha256(const std::string& data);
    };

}

