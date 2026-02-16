#include "pch.h"

namespace pokemon {
    std::mutex ResourceManager::mutex;

 ResourceManager &ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

    void ResourceManager::addPicture(const std::string &pictureName, const std::string &extention, const std::string &pictureHash,
                              const std::string &owner) {
        std::lock_guard<std::mutex> lock(mutex);
        pictureList_mp.insert({pictureName, {owner, extention, pictureHash}});
    }

    void ResourceManager::tracePicturesList(std::ostream &os) const {
        std::lock_guard<std::mutex> lock(mutex);
        trace.print(os, " -- Liste des images -- ");
        for (auto &picture: pictureList_mp)
            trace.print(os, picture.first + " -> { " + std::get<0>(picture.second)
                            + " , " + std::get<1>(picture.second) + " , " + std::get<2>(picture.second) + " }");
        trace.print(os, "Size: " + std::to_string(pictureList_mp.size()));
    }

    void ResourceManager::printPokemonPictures(std::ostream &os) const {
        std::lock_guard<std::mutex> lock(mutex);
        os <<std::endl;
        os << " -- Liste des images -- " << std::endl;

        // <nomImage, {ipOwner, extention, pictureHash} >
        for (auto &picture: pictureList_mp)
            os << picture.first << " : " << std::get<2>(picture.second) << std::endl;
        os << "Size: " + std::to_string(pictureList_mp.size()) << std::endl<<std::endl;
    }

    std::optional<std::tuple<std::string, std::string, std::string>>
    ResourceManager::findPicture(const std::string &name) const {
        std::lock_guard<std::mutex> lock(mutex);

        auto picture = pictureList_mp.find(name);
        if (picture != pictureList_mp.end()) // Si l'image est trouvée, retourner un tuple avec les valeurs appropriées
            return picture->second;
        else // Si l'image n'est pas trouvée, retourner un std::nullopt*
            return std::nullopt;
    }

    std::unordered_map<std::string, std::tuple<std::string, std::string, std::string>>
    ResourceManager::getPicturesList() const {
        std::lock_guard<std::mutex> lock(mutex);
        return pictureList_mp;
    }


    int ResourceManager::savedPictureToDisk(const std::string &location,  const std::string &pictureName, std::string &extension, const std::string &pic_str) {
        std::lock_guard<std::mutex> lock(mutex);
        try{
            if (!std::filesystem::is_directory(location) || !std::filesystem::exists(location))
                std::filesystem::create_directory(location);

            std::ofstream MyPic(location + pictureName + "." + extension);
            MyPic << pic_str;
            MyPic.close();
        }
        catch (const std::exception &e){
            trace.print(std::cerr, "Error can't save < " + pic_str + " > " );
            return 1;
        }
        return 0;
    }


    void ResourceManager::addPicturePath(const std::string &picturePath) {
        picturePath_s = picturePath;
    }

    std::tuple<std::string, std::string> ResourceManager::getNameAndExtension(const std::string &pictureHash) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = pictureList_mp.find(pictureHash);
        if (it != pictureList_mp.end()) {
            const auto &info = it->second;
            return std::make_tuple(std::get<0>(info), std::get<1>(info)); // Récupère l'extension et le nom
        }
        return std::make_tuple("", ""); // Retourne des chaînes vides si l'imageHash n'est pas trouvée
    }

    std::string ResourceManager::getPic_str(const std::string &picHash) {
        std::lock_guard<std::mutex> lock(mutex);
        std::ifstream file(picturePath_s + picHash);
        std::string out = "";
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line))
                out += line + "\n";
            file.close();
        } else
            trace.print(std::cerr, "impossible de lire");
        return out;
    }
}
