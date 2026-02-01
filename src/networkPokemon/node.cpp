#include "pch.h"

using namespace std::chrono_literals;

namespace pokemon {
    Node::Node() noexcept : m_node_info(std::make_unique<Node_Info>()) {
       /*
        if (nodei.has_value()) {
            node_info = std::make_unique<Node_Info>();
        }*/

    }

    void Node::initialized(std::string path, [[maybe_unused]] const std::string &picturePath, [[maybe_unused]] const std::string &nodeFile) noexcept{

        storagePath_s = path;
        auto node_info_data = Json::loadJson<Node_Info>(storagePath_s, NODE_INFO_FILE);

        if (node_info_data.has_value()) {
            m_node_info = std::make_unique<Node_Info>(node_info_data.value());
        }
        else {
            Node_Info default_node_info;
            default_node_info.set_name(Node_Info::DEFAULT_NODE_NAME);
            m_node_info = std::make_unique<Node_Info>(default_node_info);
            Json::saveJson<Node_Info>(storagePath_s, NODE_INFO_FILE, *m_node_info);
        }

       /* std::string fileNameNode = picturePath + nodeFile;
        addNodesList(fileNameNode);
        std::string fileNameImages = picturePath + "pokemons.txt";
        addImagesList(fileNameImages);

        resourceManager.addPicturePath(picturePath);

        sockpp::initialize();

        listen = std::make_unique<Listen>(port_s);
        client = std::make_unique<Client>(ip_s, port_s);*/
    }

     void Node::set_node_info(std::string_view node_name) noexcept {
        if (m_node_info == nullptr)
            m_node_info =  std::make_unique<Node_Info>();

        m_node_info->set_name(node_name);
        Json::saveJson<Node_Info>(storagePath_s, NODE_INFO_FILE, *m_node_info);
    }

   /* void Node::addNodesList(const std::string &fileName) {
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
                if (isValidIPAddress(line))
                    resourceManager.addNode(line);
            file.close();
        } else {
            trace.print(std::cerr, "Impossible d'ouvrir le fichier.");
            return;
        }
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
        auto nodesList = node.resourceManager.getNodesList();
        if (!node.resourceManager.empty(nodesList))
            os << "Liste Nodes connus: " << std::endl;

        for (const auto& n: nodesList)
            os << n << std::endl;

        return os;
    }
*/

    inline void to_json(nlohmann::json& j, const Node_Info& n) {
        j = nlohmann::json{
            {"nodeName", n.get_name()},
        };
    }

    inline void from_json(const nlohmann::json& j, Node_Info& n) {
        n.set_name(j.at("nodeName").get<std::string>());
    }
}
