#include "pch.h"

namespace pokemon {
     image_repository::image_repository(std::string_view storagePath)
        : storagePath_(storagePath) {}

    void image_repository::add_image(const Image& image) noexcept {
        auto im = find_image(image.get_hash());
        if (im.has_value()) {
            return;
        }
        std::unique_lock lock(mutex_);
        images_.push_back(image);
    }

    void image_repository::add_image(std::shared_ptr<Image> image) noexcept {
         auto im = find_image(image->get_hash());
         if (im.has_value()) {
             return;
         }
         std::unique_lock lock(mutex_);
         images_.push_back(*image);
     }

    std::optional<Image> image_repository::find_image(std::string_view hash) const noexcept {
        std::shared_lock lock(mutex_);
        auto it = std::find_if(images_.begin(), images_.end(),
            [&](const Image& img){ return img.get_hash() == hash; });

        if (it != images_.end()) return *it;
        return std::nullopt;
    }

    bool image_repository::save_image_to_disk(const std::string& filename, const std::string& data) const {
        try {
            std::filesystem::path path = std::filesystem::path(storagePath_) / filename;
            std::ofstream file(path, std::ios::binary);
            file << data;
            return true;
        } catch (...) {
            return false;
        }
    }

    std::shared_ptr<Image> image_repository::save_image(std::string_view name, std::string_view owner_id, std::filesystem::path image_to_save_path) noexcept {
        try {

            if (std::filesystem::exists(image_to_save_path) && std::filesystem::is_regular_file(image_to_save_path)) {
                std::ifstream file(image_to_save_path, std::ios::binary | std::ios::ate);

                if (!file.is_open()) {
                //         trace.print(std::cerr, "Impossible de lire l'image : " + image_to_save_path.to);
                    return nullptr;
                }

                std::string extension = image_to_save_path.extension().string();
                auto bytes = std::filesystem::file_size(image_to_save_path);

                double mb = static_cast<double>(bytes) / (1024.0 * 1024.0);


                std::stringstream stream;
                stream << std::fixed << std::setprecision(1) << mb << " MB";

                std::string picture_size = stream.str();

                std::string hash = name.data() + extension;

                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);

                if (size <= 0) return nullptr ;

                std::string buffer(size, '\0');
                if (file.read(&buffer[0], size)) {
                    std::string saved_name = std::format("{}{}", storagePath_, hash);
                    std::filesystem::path pathr(saved_name);

                    std::ofstream image_saved(pathr, std::ios::binary);
                    if (!image_saved.is_open()) {
                        // trace.print(std::cerr, "Impossible de sauvegarder l'image : " + pathr.string());
                        return nullptr;
                    }
                    image_saved.write(buffer.c_str(), buffer.size());
                    image_saved.close();

                    auto image = std::make_shared<Image>(name, extension, hash, owner_id);

                    add_image(image);
                    return image;
                }
            }
        } catch (const std::exception &e) {
            return nullptr;
        }

    }

    std::shared_ptr<Image> image_repository::add_picture_from_path(std::string_view name, std::string_view owner_id, std::string_view picturePath) noexcept {
         try {
             std::filesystem::path path(picturePath);
             return save_image(name,owner_id, path);

         } catch (const std::exception &e) {
             //  trace.print(std::cerr, "Erreur lors de l'ajout de l'image depuis le chemin : " + picturePath + " - " + e.what());
             return nullptr;
         }
     }

}
