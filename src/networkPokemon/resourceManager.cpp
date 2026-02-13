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

    void ResourceManager::addNode(const std::string &ip) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = std::find(nodesList_v.begin(), nodesList_v.end(), ip);

        // Si l'adresse IP n'est pas trouvée, l'ajouter
        if (it == nodesList_v.end()) {
            nodesList_v.push_back(ip);
        }
    }

    void ResourceManager::addNode(Node_Info nodeInfo) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = std::find_if(nodesInfoList_v.begin(), nodesInfoList_v.end(),
            [&nodeInfo](const Node_Info& n) {
                return n.get_name() == nodeInfo.get_name() &&
                       n.get_port() == nodeInfo.get_port();
            });

        if (it == nodesInfoList_v.end()) {
            nodesInfoList_v.push_back(nodeInfo);
        }
    }


    std::vector<std::string> ResourceManager::getNodesList() const {
        std::lock_guard<std::mutex> lock(mutex);
        return nodesList_v;
    }

    void ResourceManager::set_node_alive(std::string_view ip, const int port, bool isOnline) {
        std::lock_guard<std::mutex> lock(mutex);
        auto node = std::find_if(nodesInfoList_v.begin(), nodesInfoList_v.end(),
           [&ip, &port](const Node_Info& n) {
                return n.get_ip() == ip && n.get_port() == port;
            });
        if (node != nodesInfoList_v.end()) {
             node->set_isConnected(isOnline);
        }
    }

    std::vector<Node_Info> ResourceManager::getNodesInfoList() const {
        std::lock_guard<std::mutex> lock(mutex);
        return nodesInfoList_v;
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


    void ResourceManager::printNodesList(std::ostream &os) const {
        std::lock_guard<std::mutex> lock(mutex);
        os <<std::endl;
        os << " -- Liste des nodes -- " << std::endl;
        for (auto &node: nodesInfoList_v)
            os << node << std::endl;
        os << "nombres totals: " + std::to_string(nodesInfoList_v.size()) << std::endl <<std::endl;
    }

    std::optional<std::string> ResourceManager::findNode(const std::string &ip) const {
        std::lock_guard<std::mutex> lock(mutex);

        auto node = std::find(nodesList_v.begin(), nodesList_v.end(), ip);
        if (node != nodesList_v.end()) // Si le est trouvée, retourner l'ip
            return *node;
        // Si le node n'est pas trouvée, retourne nullopt
        return std::nullopt;
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
