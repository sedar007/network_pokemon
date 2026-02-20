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

            if (!std::filesystem::exists(image_to_save_path) && !std::filesystem::is_regular_file(image_to_save_path))
                    return nullptr;

            std::ifstream file(image_to_save_path, std::ios::binary | std::ios::ate);

            if (!file.is_open()) {
            //         trace.print(std::cerr, "Impossible de lire l'image : " + image_to_save_path.to);
                return nullptr;
            }

            std::string extension = image_to_save_path.extension().string();
            auto bytes = std::filesystem::file_size(image_to_save_path);

            double mb = static_cast<double>(bytes) / (1024.0 * 1024.0);


            std::stringstream stream;
            stream << std::fixed << std::setprecision(1) << mb;

            std::string picture_size = stream.str();
            std::string size_unit = "MB";

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            if (size <= 0) return nullptr ;

            std::string buffer(size, '\0');
            if (file.read(&buffer[0], size)) {
                const std::string hash = calculate_sha256(buffer);
                std::string saved_name = std::format("{}{}", storagePath_, hash);
                std::filesystem::path pathr(saved_name);

                std::ofstream image_saved(pathr, std::ios::binary);
                if (!image_saved.is_open()) {
                    return nullptr;
                }
                image_saved.write(buffer.c_str(), buffer.size());
                image_saved.close();

                auto image = std::make_shared<Image>(name, extension, hash, owner_id, picture_size, size_unit);

                add_image(image);
                return image;
            }
        } catch (const std::exception&) {
            return nullptr;
        }
        return nullptr;
    }

    std::shared_ptr<Image> image_repository::add_picture_from_path(std::string_view name, std::string_view owner_id, std::string_view picturePath) noexcept {
         try {
             std::filesystem::path path(picturePath);
             return save_image(name,owner_id, path);

         } catch (const std::exception) {
             //  trace.print(std::cerr, "Erreur lors de l'ajout de l'image depuis le chemin : " + picturePath + " - " + e.what());
             return nullptr;
         }
    }
#if 0
    std::string image_repository::calculate_sha256(const std::string& data) {
         unsigned char hash[SHA256_DIGEST_LENGTH];
         SHA256_CTX sha256;
         SHA256_Init(&sha256);
         SHA256_Update(&sha256, data.c_str(), data.size());
         SHA256_Final(hash, &sha256);

         std::stringstream ss;
         for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
             ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
         }
         return ss.str();
    }
#endif
    std::string image_repository::calculate_sha256(const std::string& data) {
         unsigned char hash[EVP_MAX_MD_SIZE];
         unsigned int lengthOfHash = 0;


         EVP_MD_CTX* context = EVP_MD_CTX_new();

         if (context != nullptr) {
             if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
                 EVP_DigestUpdate(context, data.c_str(), data.size());
                 EVP_DigestFinal_ex(context, hash, &lengthOfHash);
             }
             EVP_MD_CTX_free(context);
         }

         std::stringstream ss;
         for(unsigned int i = 0; i < lengthOfHash; i++) {
             ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
         }
         return ss.str();
    }

    std::string image_repository::getPic_str(const Image image) {

         std::string fullPath = std::format("{}{}", storagePath_, image.get_hash());
         std::ifstream file(fullPath, std::ios::binary | std::ios::ate);

         if (!file.is_open()) {
             return "";
         }

         std::streamsize size = file.tellg();
         file.seekg(0, std::ios::beg);

         if (size <= 0) return "";

         std::string buffer(size, '\0');
         if (file.read(&buffer[0], size)) {
             return buffer;
         }

         return "";
     }


    std::string image_repository::get_picture_base64(const Image image) {
         std::string rawData = getPic_str(image);
         if (rawData.empty()) {
             return "";
         }
         std::string base64Data = base64_encode(rawData);
         return "data:image/png;base64," + base64Data;
     }

    std::string image_repository::base64_encode(const std::string &in) noexcept {
         std::string out;
         int val = 0, valb = -6;
         for (unsigned char c : in) {
             val = (val << 8) + c;
             valb += 8;
             while (valb >= 0) {
                 out.push_back(BASE64_CHARS[(val >> valb) & 0x3F]);
                 valb -= 6;
             }
         }
         if (valb > -6) out.push_back(BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
         while (out.size() % 4) out.push_back('=');
         return out;
     }

}
