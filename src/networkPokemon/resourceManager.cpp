#include "pch.h"

namespace pokemon {
    std::mutex ResourceManager::mutex;

 ResourceManager &ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}



    void ResourceManager::tracePicturesList(std::ostream &os) const {
        std::lock_guard<std::mutex> lock(mutex);
        trace.print(os, " -- Liste des images -- ");
       /* for (auto &picture: pictureList_mp)
            trace.print(os, picture.first + " -> { " + std::get<0>(picture.second)
                            + " , " + std::get<1>(picture.second) + " , " + std::get<2>(picture.second) + " }");
        trace.print(os, "Size: " + std::to_string(pictureList_mp.size()));*/
    }

    void ResourceManager::printPokemonPictures(std::ostream &os) const {
        std::lock_guard<std::mutex> lock(mutex);
        os <<std::endl;
        os << " -- Liste des images -- " << std::endl;

        // <nomImage, {ipOwner, extention, pictureHash} >
       /* for (auto &picture: pictureList_mp)
            os << picture.first << " : " << std::get<2>(picture.second) << std::endl;
        os << "Size: " + std::to_string(pictureList_mp.size()) << std::endl<<std::endl;*/
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

}
