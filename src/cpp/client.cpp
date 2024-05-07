#include "../hpp/client.hpp"
#include <iostream>
#include "chrono"
#include "exception"
#include "../hpp/const.hpp"
using namespace std::chrono_literals;
namespace pokemon {

    Client::Client(const std::string &ip, const in_port_t port)
            : port_s(port), ip_s(ip) {
        auto run = getIps();
        run.detach();
    }

    std::thread Client::run(const std::string &neighbour_ip, const in_port_t neighbour_port, const std::string &msg) {
        return std::thread([this, neighbour_ip, neighbour_port, &msg] { start(neighbour_ip, neighbour_port, msg); });
    }

    void Client::run_getIp() {
        bool getIp = true;
        while (true) {
            std::string msg;
            for (auto &node: resourceManager.getNodesList()) {
                std::string neighbour_ip;
                in_port_t neighbour_port;
                if(getPort_Ip(node, neighbour_ip, neighbour_port) == -1)
                    continue;
                if (getIp)
                    msg = protocolToString(PROTOCOLE::GET_IPS);
                else
                    msg = protocolToString(PROTOCOLE::GET_PICS);

                auto clientThread = run(neighbour_ip, neighbour_port, msg);
                clientThread.detach();
                std::this_thread::sleep_for(threadSleep_s(500, 1500));
            }
            getIp = !getIp;
            std::this_thread::sleep_for(threadSleep_s(1000, 3000));
        }
    }

    std::thread Client::getIps() {
        return std::thread([this] { run_getIp(); });
    }

    void Client::getPic(const std::string &pictureName) {

        auto pic = resourceManager.findPicture(pictureName);

        if (pic.has_value()) { //vérifie si l'optional contient une valeur

            //<nomImage, {ipOwner, extention, imageHash} >
            auto tupleValue = pic.value();
            std::string ipOwner = std::get<0>(tupleValue);
            std::string extention = std::get<1>(tupleValue);
            std::string pictureHash = std::get<2>(tupleValue);

            if (ipOwner == (ip_s + ":" + std::to_string(port_s))) {
                auto pic_str = resourceManager.getPic_str(pictureHash);
                if (pic_str.empty()) {
                    std::cout << NO_PICTURE_FOUND(pictureName) << std::endl;
                    trace.print(std::cerr, NO_PICTURE_FOUND(pictureName));
                    return;
                }
                if(resourceManager.savedPictureToDisk(FOLDER_TO_SAVE,pictureName , extention, pic_str) == 0)
                    std::cout<<SAVE_PICTURE_SUCCESS<<std::endl;
                return;
            }

            std::string knownNodeIp;
            in_port_t knownNodePort;
            if(getPort_Ip(ipOwner, knownNodeIp, knownNodePort) == -1){
                std::cout << SAVE_PICTURE_FAIL << std::endl;
                trace.print(std::cerr, "Can't extract Port or Ip");
                return;
            }

            // Protocole  + sizePictureHash + pictureHash +  sizePictureName  + pictureName + sizeExtention  + extention
            std::string msg = protocolToString(PROTOCOLE::GET_PIC) + generateFormattedNumber(pictureHash.size()) + pictureHash +
                    generateFormattedNumber(pictureName.size()) + pictureName + generateFormattedNumber(extention.size()) + extention;
            auto clientThread = run(knownNodeIp, knownNodePort, msg);
            clientThread.join();
        }
        else{
            std::cout << NO_PICTURE_FOUND(pictureName) << std::endl;
            trace.print(std::cerr, NO_PICTURE_FOUND(pictureName));
        }
    }


    int Client::start(const std::string &neighbour_ip, const in_port_t neighbour_port, const std::string &msg) {
        std::string node_id = std::to_string(port_s);
        std::string nodeIdStr("[node - " + node_id + "]");
        std::string knowPortStr(std::to_string(neighbour_port));
        sockpp::tcp_connector connector;

        trace.print(std::clog, nodeIdStr +" - client starting to connect to "+ knowPortStr +"...");

        std::this_thread::sleep_for(threadSleep_s(500, 1000));

        if (auto res = connector.connect(neighbour_ip, neighbour_port, 5s); !res) {
            trace.print(std::cerr,
                        nodeIdStr + " - Error connecting to server at: " + neighbour_ip + " on port " +
                        std::to_string(neighbour_port) + " : " + res.error_message());
           connector.shutdown(SHUT_RDWR);
            return 1;
        }
        trace.print(std::clog,nodeIdStr +" - Created a connection to " + knowPortStr);


        // Send
        if (auto res = connector.write(msg); res != msg.size()) {
            trace.print(std::cerr,
                        nodeIdStr +" - Error writing to the TCP stream: " + res.error_message());
            connector.shutdown(SHUT_RDWR);
            return 1;
        }


        // read
        char sizeMsg[getFormattedNumberSize()]; // Recupere la taille du buffer
        if (auto res = connector.read(sizeMsg, getFormattedNumberSize()); !res) {
            trace.print(std::cerr, nodeIdStr + " - Error reading the buffer Size");
            connector.shutdown(SHUT_RDWR);
            return 1;
        }


        // Le type de requete
        char queryBuf[protocolSize()]; // Recupere le protocol
        if (auto res = connector.read(queryBuf, protocolSize()); !res) {
            trace.print(std::cerr, nodeIdStr + " - Error reading the message");
            connector.shutdown(SHUT_RDWR);
            return 1;
        }

        std::string protocole(queryBuf, protocolSize());
        memset(queryBuf,0, protocolSize());

        size_t t;
        try {
            t = std::stoi(sizeMsg); // Convert la taille du message
        }
        catch (const std::exception &e) {
            trace.print(std::cerr, nodeIdStr + " - Error can't convert " + sizeMsg + " to a integer");
            connector.shutdown(SHUT_RDWR);
            return 1;
        }

        char msg_buf[t];

        if (auto res = connector.read(msg_buf, t); !res) { // Recupere le message envoyer
            trace.print(std::cerr, nodeIdStr + " - Error reading the message");
            connector.shutdown(SHUT_RDWR);
            return 1;
        }

        std::string msg_str(msg_buf, t);
        memset(msg_buf,0, protocolSize());

        if (protocole == protocolToString(PROTOCOLE::GET_IPS)) {
            addIps(msg_str, nodeIdStr);
        }

        else if (protocole == protocolToString(PROTOCOLE::GET_PICS)) {
            addPictures(msg_str);
        }

        else if (protocole == protocolToString(PROTOCOLE::GET_PIC)) {
            addPicture(msg_str, nodeIdStr);
        }

        trace.print(std::clog, nodeIdStr + " - Connection Closed from " + std::to_string(neighbour_port));
        connector.shutdown(SHUT_RDWR);
        return (!connector) ? -1 : 0;
    }


    void Client::addIps(const std::string &ips_str, const std::string &id) const {
        std::string tmp;
        std::string ip;
        std::stringstream ss(ips_str);
        while (std::getline(ss, ip, ';')) {
            if (!isValidIPAddress(ip)) {
                trace.print(std::cerr, id +" - Ip Invalid: " + ip);
                continue;
            }

            if (ip != getAdress(ip_s, port_s))
                resourceManager.addNode(ip);
        }
    }

    void Client::addPictures(const std::string &str) const {
        std::string pic_str;
        std::stringstream ss(str);
        while (std::getline(ss, pic_str, ';')) {
            std::string nomImage;
            std::string extention;
            std::string imageHash;
            std::string owner;

            std::stringstream ss2(pic_str);

            // Utilisation de getline pour extraire chaque partie en utilisant ':' comme délimiteur
            getline(ss2, nomImage, ',');
            getline(ss2, owner, ',');
            getline(ss2, extention, ',');
            getline(ss2, imageHash);
            resourceManager.addPicture(nomImage, extention, imageHash, owner);
        }
    }

    void Client::addPicture(const std::string &str, const std::string &nodeIdStr) const{

        size_t pos = 0;

        std::string name_size_buf(str, pos, getFormattedNumberSize());

        pos += getFormattedNumberSize();
        size_t t;

        try {
            t  = std::stoi(name_size_buf);
        }
        catch (const std::exception &e){
              std::cout<<SAVE_PICTURE_FAIL<<std::endl;
              trace.print(std::cerr, nodeIdStr + " - Error can't convert " + name_size_buf + " to a integer");
        }

        std::string picName(str, pos, t);
        pos += t;
        std::string extension_size(str, pos, getFormattedNumberSize());
        pos += getFormattedNumberSize();
        try {
            t = std::stoi(extension_size);
        }
        catch (const std::exception &e){
            std::cout<<SAVE_PICTURE_FAIL<<std::endl;
            trace.print(std::cerr, nodeIdStr + " - Error can't convert " + extension_size + " to a integer");
        }

        std::string extention(str, pos, t);
        pos += t;
        t = str.size() - pos;
        std::string pic_str(str, pos, t);

        if(resourceManager.savedPictureToDisk(FOLDER_TO_SAVE , picName , extention, pic_str) == 0)
            std::cout<< SAVE_PICTURE_SUCCESS<<std::endl;
        else
            std::cout<< SAVE_PICTURE_FAIL<<std::endl;
    }

}