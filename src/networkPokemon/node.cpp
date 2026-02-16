#include "pch.h"
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>

using namespace std::chrono_literals;

namespace pokemon {
    Node::Node() noexcept {
    }

    void Node::initialized(std::string_view path) noexcept{
        storagePath_s = path;
        resourceManager.set_path(path);
        std::cout << "Node::initialized() - Storage path set to: " << storagePath_s << std::endl;
        auto node_info_data = Json::loadJson<Node_Info>(storagePath_s, NODE_INFO_FILE);

        if (node_info_data.has_value()) {
            m_node_info = std::make_shared<Node_Info>(node_info_data.value());
            m_node_info->set_ip(get_network_ip());
        }
        else {
            m_node_info = std::make_shared<Node_Info>(Helper::generate_uuid_v4(), Node_Info::DEFAULT_NODE_NAME, get_network_ip(), find_available_port(DEFAULT_PREFERRED_PORT));
        }

        Json::saveJson<Node_Info>(storagePath_s, NODE_INFO_FILE, *m_node_info);

        addNodesList();


        addImagesList();

        /*std::string fileNameNode = path + nodeFile;
        addNodesList(fileNameNode);*/
        std::string fileNameImages = std::format("{}{}", path, "pokemons.txt");
        addImagesList(fileNameImages);

        resourceManager.addPicturePath(path.data());

        sockpp::initialize();

        listen = std::make_unique<Listen>(m_node_info->get_port(), m_node_info);
        client = std::make_unique<Client>(m_node_info->get_ip(), m_node_info->get_port(), m_node_info);
    }

    std::string Node::get_picture(const Image image) {
        std::string rawData = resourceManager.getPic_str(image, storagePath_s);

        if (rawData.empty()) {
            return "";
        }

        std::string base64Data = base64_encode(rawData);
        return "data:image/png;base64," + base64Data;
    }


     void Node::set_node_info(std::string_view node_name) noexcept {
        if (m_node_info == nullptr)
            m_node_info =  std::make_unique<Node_Info>();

        m_node_info->set_name(node_name);
        Json::saveJson<Node_Info>(storagePath_s, NODE_INFO_FILE, *m_node_info);
    }

    std::string Node::get_network_ip() const {
        struct ifaddrs *interfaces = nullptr;
        struct ifaddrs *temp_addr = nullptr;
        std::string ipAddress = LOCALHOST_IP.data();
        int success = 0;

        success = getifaddrs(&interfaces);

        if (success == 0) {
            temp_addr = interfaces;
            while (temp_addr != nullptr) {
                if (temp_addr->ifa_addr->sa_family == AF_INET) {
                    std::string interfaceName = temp_addr->ifa_name;
                    if (interfaceName == EN0_INTERFACE || (interfaceName != Lo_0_INTERFACE && ipAddress == LOCALHOST_IP)) {
                        char addressBuffer[INET_ADDRSTRLEN];
                        void* addrPtr = &((struct sockaddr_in*)temp_addr->ifa_addr)->sin_addr;
                        inet_ntop(AF_INET, addrPtr, addressBuffer, INET_ADDRSTRLEN);
                        ipAddress = addressBuffer;
                        if (interfaceName == EN0_INTERFACE) {
                            break;
                        }
                    }
                }
                temp_addr = temp_addr->ifa_next;
            }
        }
        freeifaddrs(interfaces);
        return ipAddress;
    }

    int Node::find_available_port(int preferred_port) {
       /* int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) return 0;

        struct sockaddr_in sin;
        std::memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(preferred_port);

        if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == 0) {
            socklen_t len = sizeof(sin);
            if (getsockname(sock, (struct sockaddr *)&sin, &len) == 0) {
                preferred_port = ntohs(sin.sin_port);
            }
            close(sock);
            return preferred_port;
        }
        close(sock);
        if (preferred_port != 0) {
            return find_available_port(0);
        }
        return 0;*/
        return preferred_port;
    }

    void Node::addNodesList(const std::string &fileName) {
        std::ifstream file(fileName);

        if (file.is_open()) {
            std::string line;
            std::getline(file, line);
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                ip_s = line.substr(0, pos);
                port_s = std::stoi(line.substr(pos + 1));
            }
            while (std::getline(file, line))
                if (isValidIPAddressPort(line))
                    resourceManager.addNode(line);
            file.close();
        } else {
            trace.print(std::cerr, "Impossible d'ouvrir le fichier.");
            return;
        }
    }

    void Node::addNodesList() {
        auto node_List = Json::loadJson<std::vector<Node_Info>>(storagePath_s, NODE_LIST_FILE);
        if (node_List.has_value()) {
            for (const auto& nodeInfo : node_List.value()) {
                resourceManager.addNode(nodeInfo);
            }
        }
    }

    void Node::addImagesList() {
        auto image_list = Json::loadJson<std::vector<Image>>(storagePath_s, IMAGE_LIST_FILE);
        if (image_list.has_value()) {
            for (const auto& image : image_list.value()) {
                resourceManager.addImage(image);
            }
        }
    }

    void Node::add_new_peer(std::string peer_name, std::string peer_ip) noexcept {

       add_peer(Helper::generate_uuid_v4(), peer_name, peer_ip, find_available_port(DEFAULT_PREFERRED_PORT));
    }

    void Node::add_peer(std::string id, std::string peer_name, std::string peer_ip, int port) noexcept {
        client->add_new_node(peer_name, peer_ip, port);
        /*Node_Info node_info(id, peer_name, peer_ip, port);
        resourceManager.addNode(node_info);
        auto node_List = Json::loadJson<std::vector<Node_Info>>(storagePath_s, NODE_LIST_FILE);
        if (node_List.has_value()) {
            auto nodes = node_List.value();
            nodes.push_back(node_info);
            Json::saveJson<std::vector<Node_Info>>(storagePath_s, NODE_LIST_FILE, nodes);
        } else {
            std::vector<Node_Info> nodes;
            nodes.push_back(node_info);
            Json::saveJson<std::vector<Node_Info>>(storagePath_s, NODE_LIST_FILE, nodes);
        }*/
    }

    void Node::addImagesList(const std::string &fileName) {
        std::ifstream file(fileName);
        char separateur = ' ';
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string name, extension, hash;
                std::getline(ss, name, separateur);
                std::getline(ss, extension, separateur);
                std::getline(ss, hash, separateur);
                resourceManager.addPicture(name, extension, hash, ip_s + ":" + std::to_string(port_s));
            }
            file.close();
        } else {
            trace.print(std::cerr, "Impossible d'ouvrir le fichier.");
            return;
        }
    }


    std::ostream &operator<<(std::ostream &os, const Node &node) {
        os << "[ Node " << " ] : " << "Ip: " << node.ip_s << " port: " << node.port_s << std::endl;
        auto nodesList = node.resourceManager.getNodesInfoList();
        if (!node.resourceManager.empty(nodesList))
            os << "Liste Nodes connus: " << std::endl;

      /* for (const auto& n: nodesList)
            os << n << std::endl;*/

        return os;
    }


    void Node::add_pokemon(std::string_view name, std::string_view picturePath) noexcept {
        std::shared_ptr<Image> image = resourceManager.addPictureFromPath(name, picturePath, storagePath_s);
        if (image == nullptr) {
            trace.print(std::cerr, "Erreur lors de l'ajout de l'image depuis le chemin : ", picturePath.data());
            return;
        }

        auto image_list = Json::loadJson<std::vector<Image>>(storagePath_s, IMAGE_LIST_FILE);
        std::vector<Image> images_list_to_save;
        if (image_list.has_value()) {
            images_list_to_save = image_list.value();
        }
        images_list_to_save.push_back(*image);
        Json::saveJson(storagePath_s.data(), IMAGE_LIST_FILE.data(), images_list_to_save);
    }

    void Node::remove_pokemon(std::string_view name, std::string_view picturePath) noexcept {
        /*std::shared_ptr<Image> image = resourceManager.addPictureFromPath(name, picturePath, storagePath_s);
        if (image == nullptr) {
            trace.print(std::cerr, "Erreur lors de l'ajout de l'image depuis le chemin : ", picturePath.data());
            return;
        }

        auto image_list = Json::loadJson<std::vector<Image>>(storagePath_s, IMAGE_LIST_FILE);
        std::vector<Image> images_list_to_save;
        if (image_list.has_value()) {
            images_list_to_save = image_list.value();
        }
        images_list_to_save.push_back(*image);
        Json::saveJson(storagePath_s.data(), IMAGE_LIST_FILE.data(), images_list_to_save);*/
    }


    inline void to_json(nlohmann::json& j, const Node_Info& n) {
        j = nlohmann::json{
            {Node_Info::NODE_ID_KEY, n.get_id()},
            {Node_Info::NODE_NAME_KEY, n.get_name()},
            {Node_Info::NODE_PORT_KEY, n.get_port()},
            {Node_Info::NODE_IP_KEY, n.get_ip()}
        };
    }

    inline void from_json(const nlohmann::json& j, Node_Info& n) {
        n.set_id(j.at(Node_Info::NODE_ID_KEY).get<std::string>());
        n.set_name(j.at(Node_Info::NODE_NAME_KEY).get<std::string>());
        n.set_port(j.at(Node_Info::NODE_PORT_KEY).get<int>());
        n.set_ip(j.at(Node_Info::NODE_IP_KEY).get<std::string>());
    }

    inline void to_json(nlohmann::json& j, const Image& i) {
        j = nlohmann::json{
                {Image::IMAGE_NAME_KEY, i.get_name()},
                {Image::IMAGE_EXTENSION_KEY, i.get_extension()},
                {Image::IMAGE_HASH_KEY, i.get_hash()},
        };
    }

    inline void from_json(const nlohmann::json& j, Image& i) {
      i.set_name(j.at(Image::IMAGE_NAME_KEY).get<std::string>());
      i.set_extension(j.at(Image::IMAGE_EXTENSION_KEY).get<std::string>());
      i.set_hash(j.at(Image::IMAGE_HASH_KEY).get<std::string>());
    }
}
