#pragma once

namespace pokemon {
    class Json {
    public:template<typename T>
    static inline void saveJson(std::string_view storagePath, std::string_view filename, const T& data) {
        if (storagePath.empty()) return;

        try {
            nlohmann::json j = data;
            std::string fullPath = std::string(storagePath) + "/" + std::string(filename);
            std::ofstream file(fullPath);
            if (file.is_open()) {
                file << j.dump(4);
                file.close();
            }
        } catch (const std::exception& e) {
            // Gestion d'erreur optionnelle
        }
    }

        template<typename T>
        static inline std::optional<T> loadJson(std::string_view storagePath, std::string_view filename) {
        if (storagePath.empty()) return std::nullopt;

        try {
            std::string fullPath = std::string(storagePath) + "/" + std::string(filename);
            std::ifstream file(fullPath);
            if (file.is_open()) {
                nlohmann::json j;
                file >> j;
                return j.get<T>();
            }
        } catch (const std::exception& e) {
            // Gestion d'erreur
        }
        return std::nullopt;
    }

    };
}
