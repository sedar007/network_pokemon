#include "pch.h"

namespace pokemon {

    Client::Client(const std::string &ip, const in_port_t port) noexcept
            : port_s(port), ip_s(ip) {
        auto run = getIps();
        run.detach();
    }

    std::thread Client::run(const std::string &neighbour_ip, const in_port_t neighbour_port, const std::string &msg) noexcept {
        return std::thread([this, neighbour_ip, neighbour_port, &msg] { start(neighbour_ip, neighbour_port, msg); });
    }

    void Client::run_getIp() noexcept {
        bool getIp = true;
        while (true) {
            std::string msg;
            for (auto &node: resourceManager.getNodesList()) {
                std::string neighbour_ip;
                in_port_t neighbour_port;
                if(getPort_Ip(node, neighbour_ip, neighbour_port) == -1)
                    continue;
                if (getIp)
                    msg = protocolToString(PROTOCOL::GET_IPS);
                else
                    msg = protocolToString(PROTOCOL::GET_PICS);

                auto clientThread = run(neighbour_ip, neighbour_port, msg);
                clientThread.detach();
                std::this_thread::sleep_for(threadSleep_s(500, 1500));
            }
            getIp = !getIp;
            std::this_thread::sleep_for(threadSleep_s(1000, 3000));
        }
    }

    std::thread Client::getIps() noexcept {
        return std::thread([this] { run_getIp(); });
    }

    void Client::getPic(const std::string &pictureName) noexcept {

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
            std::string msg = protocolToString(PROTOCOL::GET_PIC) + generateFormattedNumber(pictureHash.size()) + pictureHash +
                    generateFormattedNumber(pictureName.size()) + pictureName + generateFormattedNumber(extention.size()) + extention;
            auto clientThread = run(knownNodeIp, knownNodePort, msg);
            clientThread.join();
        }
        else{
            std::cout << NO_PICTURE_FOUND(pictureName) << std::endl;
            trace.print(std::cerr, NO_PICTURE_FOUND(pictureName));
        }
    }

    int Client::start(const std::string &neighbour_ip, in_port_t neighbour_port, const std::string &msg) noexcept {

        if (port_s == neighbour_port) {
            return -1;
        }

        std::string knowPortStr(std::to_string(neighbour_port));
        sockpp::tcp_connector connector;

        trace.print(std::clog, std::format(MSG_CLIENT_TRYING_TO_CONNECT, std::format(MSG_NODE_ID, port_s, CLIENT), knowPortStr));

        std::this_thread::sleep_for(threadSleep_s(500, 1000));


        if (!connector.connect(sockpp::inet_address(neighbour_ip, neighbour_port))){
            trace.print(std::cerr, std::format(MSG_CLIENT_ERROR_CONNECTING, std::format(MSG_NODE_ID, port_s, CLIENT), neighbour_ip, neighbour_port));
            connector.shutdown(SHUT_RDWR);
        }

        trace.print(std::clog, std::format(MSG_CLIENT_CONNECTED, std::format(MSG_NODE_ID, port_s, CLIENT), neighbour_ip, neighbour_port));


        // Send
        if (auto res = connector.write(msg); res != msg.size()) {
            trace.print(std::cerr, std::format(MSG_CLIENT_ERROR_WRITING_TCP_STREAM, std::format(MSG_NODE_ID, port_s, CLIENT), connector.last_error()));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }


        // read
        char sizeMsg[FORMATTED_NUMBER_SIZE]; // Recupere la taille du buffer
        if (auto res = connector.read(sizeMsg, FORMATTED_NUMBER_SIZE); !res) {
            trace.print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM, std::format(MSG_NODE_ID, port_s, CLIENT), connector.last_error()));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }


        // Le type de requete
        char queryBuf[protocolSize()]; // Recupere le protocol
        if (auto res = connector.read(queryBuf, protocolSize()); !res) {
            trace.print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM, std::format(MSG_NODE_ID, port_s, CLIENT), connector.last_error()));
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
            trace.print(std::cerr, std::format(MSG_CLIENT_ERROR_CONVERTING_SIZE, std::format(MSG_NODE_ID, port_s, CLIENT), sizeMsg));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }

        char msg_buf[t];

        if (auto res = connector.read(msg_buf, t); !res) { // Recupere le message envoyer
            trace.print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM, std::format(MSG_NODE_ID, port_s, CLIENT), connector.last_error()));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }

        std::string msg_str(msg_buf, t);
        memset(msg_buf,0, protocolSize());

        if (protocole == protocolToString(PROTOCOL::GET_IPS)) {
            addIps(msg_str);
        }

        else if (protocole == protocolToString(PROTOCOL::GET_PICS)) {
            addPictures(msg_str);
        }

        else if (protocole == protocolToString(PROTOCOL::GET_PIC)) {
            addPicture(msg_str);
        }

       // trace.print(std::clog, nodeIdStr + " - Connection Closed from " + std::to_string(neighbour_port));
        connector.shutdown(SHUT_RDWR);
        return (!connector) ? -1 : 0;

    }


    void Client::addIps(const std::string &ips_str) const noexcept {
        std::string tmp;
        std::string ip;
        std::stringstream ss(ips_str);
        while (std::getline(ss, ip, ';')) {
            if (!isValidIPAddress(ip)) {
                trace.print(std::clog, std::format(MSG_CLIENT_ERROR_INVALID_IP, std::format(MSG_NODE_ID, port_s, CLIENT), ip));
                continue;
            }

            if (ip != getAdress(ip_s, port_s))
                resourceManager.addNode(ip);
        }
    }

    void Client::addPictures(const std::string &str) const noexcept {
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

    void Client::addPicture(const std::string &str) const noexcept{

        size_t pos = 0;

        std::string name_size_buf(str, pos, FORMATTED_NUMBER_SIZE);

        pos += FORMATTED_NUMBER_SIZE;
        size_t t;

        try {
            t  = std::stoi(name_size_buf);
        }
        catch (const std::exception &e){
              std::cout<<SAVE_PICTURE_FAIL<<std::endl;
            trace.print(std::cerr, std::format(MSG_CLIENT_ERROR_CONVERTING_SIZE, std::format(MSG_NODE_ID, port_s, CLIENT), name_size_buf));
            return;
        }

        std::string picName(str, pos, t);
        pos += t;
        std::string extension_size(str, pos, FORMATTED_NUMBER_SIZE);
        pos += FORMATTED_NUMBER_SIZE;
        try {
            t = std::stoi(extension_size);
        }
        catch (const std::exception &e){
            std::cout<<SAVE_PICTURE_FAIL<<std::endl;
            //trace.print(std::cerr, nodeIdStr + " - Error can't convert " + extension_size + " to a integer");
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
