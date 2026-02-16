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
        m_storage = std::make_unique<storage_manager>(path);
        resourceManager.set_path(path);
        std::cout << "Node::initialized() - Storage path set to: " << storagePath_s << std::endl;
        auto node_info_data = m_storage->loadNodeInfo();
        if (node_info_data.has_value()) {
            m_node_info = std::make_shared<Node_Info>(node_info_data.value());
            m_node_info->set_ip(get_network_ip());
        }
        else {
            m_node_info = std::make_shared<Node_Info>(Helper::generate_uuid_v4(), Node_Info::DEFAULT_NODE_NAME, get_network_ip(), find_available_port(DEFAULT_PREFERRED_PORT));
        }

        m_storage->saveNodeInfo(*m_node_info);
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
        if (m_storage) {
            m_storage->saveNodeInfo(*m_node_info);
        }
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
        if (!m_storage) return;

        auto nodes = m_storage->loadNodeList();
        for (const auto& nodeInfo : nodes) {
            resourceManager.addNode(nodeInfo);
        }
    }

    void Node::addImagesList() {
        if (!m_storage) return;

        auto images = m_storage->loadImageList();
        for (const auto& image : images) {
            resourceManager.addImage(image);
        }
    }

    void Node::add_new_peer(std::string peer_name, std::string peer_ip) noexcept {
       add_peer(peer_name, peer_ip, find_available_port(DEFAULT_PREFERRED_PORT));
    }

    void Node::add_peer(std::string peer_name, std::string peer_ip, int port) noexcept {
        client->add_new_node(peer_name, peer_ip, port);
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
        std::shared_ptr<Image> image = client->add_pokemon(name, picturePath);

        if (image == nullptr) {
            trace.print(std::cerr, "Erreur lors de l'ajout de l'image depuis le chemin : ", picturePath.data());
            return;
        }

        if (m_storage) {
            m_storage->addImageToSavedList(*image);
        }
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
}
