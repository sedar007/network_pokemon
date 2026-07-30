#pragma once

#include <filesystem>
#include <system_error>

namespace pokemon {
    class Json {
    public:
        template<typename T>
        static inline void saveJson(std::string_view storagePath, std::string_view filename, const T& data) {
            if (storagePath.empty()) return;

            try {
                nlohmann::json j = data;
                if (storagePath.back() == '/')
                    storagePath.remove_suffix(1);

                const std::string fullPath = std::string(storagePath) + "/" + std::string(filename);
                const std::string tmpPath = fullPath + ".tmp";

                {
                    std::ofstream file(tmpPath, std::ios::trunc);
                    if (!file.is_open()) {
                        std::cerr << "[storage] Impossible d'ouvrir " << tmpPath << " en écriture" << std::endl;
                        return;
                    }
                    file << j.dump(4);
                    if (!file) {
                        std::cerr << "[storage] Échec d'écriture dans " << tmpPath << std::endl;
                        return;
                    }
                }

                std::error_code ec;
                std::filesystem::rename(tmpPath, fullPath, ec);
                if (ec) {
                    std::cerr << "[storage] Échec du remplacement atomique de " << fullPath << ": " << ec.message() << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "[storage] Erreur lors de la sauvegarde de " << filename << ": " << e.what() << std::endl;
            }
        }

        template<typename T>
        static inline std::optional<T> loadJson(std::string_view storagePath, std::string_view filename) {
            if (storagePath.empty()) return std::nullopt;

            try {
                const std::string fullPath = std::string(storagePath) + "/" + std::string(filename);
                std::ifstream file(fullPath);
                if (file.is_open()) {
                    nlohmann::json j;
                    file >> j;
                    return j.get<T>();
                }
            } catch (const std::exception& e) {
                std::cerr << "[storage] Erreur lors du chargement de " << filename << ": " << e.what() << std::endl;
            }
            return std::nullopt;
        }

    };
}
