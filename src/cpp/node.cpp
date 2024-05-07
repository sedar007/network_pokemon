#include "../hpp/node.hpp"
#include <iostream>
#include <fstream>
#include "sstream"
#include "hpp/listen.hpp"
#include "hpp/client.hpp"
#include "sockpp/tcp_acceptor.h"
#include "sockpp/tcp_connector.h"
#include "sockpp/tcp_socket.h"

#include "chrono"

using namespace std::chrono_literals;

namespace pokemon {
    Node::Node(const std::string &picturePath, const std::string &nodeFile) {

        std::string fileNameNode = picturePath + nodeFile;
        addNodesList(fileNameNode);
        std::string fileNameImages = picturePath + "pokemons.txt";
        addImagesList(fileNameImages);

        resourceManager.addPicturePath(picturePath);

        sockpp::initialize();

        listen = std::make_unique<Listen>(port_s);
        client = std::make_unique<Client>(ip_s, port_s);


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

        for (auto &n: nodesList)
            os << n << std::endl;

        return os;
    }

    void Node::getPic(const std::string &picName) {
        client->getPic(picName);
    }

}
