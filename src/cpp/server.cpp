#include "../hpp/server.hpp"
#include <iostream>
#include "chrono"
#include <sstream>

using namespace std::chrono_literals;
namespace pokemon {

    Server::Server(in_port_t port) : port_i(port) {}

    std::thread Server::run(sockpp::tcp_socket &socket) {
        return std::thread([this, &socket] { process(socket); });
    }

    int Server::process(sockpp::tcp_socket &socket) {
        std::string id_str("[node - " + std::to_string(port_i) + "] - Server");

        size_t sizeBuf = 512;
        char buf[sizeBuf];
        sockpp::result<size_t> res;


        while ((res = socket.read(buf, sizeof(buf))) && res.value() > 0) {
            std::string socketAdress(socket.address().to_string());
            std::string std_send("");
            std::string buf_str(buf, sizeBuf);

            std::string protocoleStr(buf, protocolSize()); // get protocole
            trace.print(std::clog, id_str + " - received " + protocoleStr + " query");

            if (protocoleStr == protocolToString(PROTOCOLE::GET_IPS)) {
                std_send = getIpsToSend();
                trace.print(std::clog, id_str + " - ports list was sent to " + socketAdress);
            }
            else if (protocoleStr == protocolToString(PROTOCOLE::GET_PICS)) {
                std_send = getPicsToSend();
                trace.print(std::clog, id_str + " - Pictures list was sent to " + socketAdress);
            }
            else if (protocoleStr == protocolToString(PROTOCOLE::GET_PIC)) {
                std_send = getPicToSend(buf_str, buf);
                if(std_send.empty()){
                    memset(buf, 0, sizeBuf);
                    socket.shutdown(SHUT_RDWR);
                    return -1;
                }
                trace.print(std::clog, id_str + " - Picture was sent to " + socketAdress);
            }

            // Renvoie une chaine de format 00XX + receivedData + std_send
            // Pour pouvoir connaitre la taille de std_send pour creer mon buf cote client
            // Et pouvoir idendifier la requete
            std_send = generateFormattedNumber(std_send.size()) + protocoleStr + std_send;
            socket.write(&std_send[0], std_send.size());
            break;
        }
        memset(buf, 0, sizeBuf);
        socket.shutdown(SHUT_RDWR);
        return 0;
    }

    std::string Server::getIpsToSend() const {
        std::string str("");
        for (auto &msg: resourceManager.getNodesList())
            str += msg + ";";
        return str;
    }

    std::string Server:: getPicsToSend() const {
        std::string str("");
        for (auto &msg: resourceManager.getPicturesList())
            str += msg.first + "," + std::get<0>(msg.second) + ","
                   + std::get<1>(msg.second) + ","
                   + std::get<2>(msg.second) + ";";
        return str;
    }

    std::string Server::getPicToSend(const std::string &buf_str, const char *buf) const{
        // sizePictureHash + pictureHash +  sizePictureName  + pictureName + sizeExtention  + extention
        std::string str("");
        size_t pos = protocolSize();

        std::string sizeHash_str = buf_str.substr(pos, getFormattedNumberSize());  // sizePictureHash : str
        pos += getFormattedNumberSize();
        trace.print(std::clog, sizeHash_str);
        size_t sizeHash;
        try {
            sizeHash = std::stoi(sizeHash_str); // sizePictureHash : size_t
            std::string pic_hash = buf_str.substr(pos, sizeHash); // pictureHash : str
            pos += sizeHash;
            trace.print(std::clog, pic_hash);

            size_t a = pos;
            std::string sizePicName_str = buf_str.substr(pos, getFormattedNumberSize()); // sizePictureName : str
            pos += getFormattedNumberSize();
            size_t sizePicName = std::stoi(sizePicName_str); // sizePictureName : size_t
            pos += sizePicName;

            std::string sizePicExtension_str = buf_str.substr(pos, getFormattedNumberSize()); // sizeExtention : str
            pos += getFormattedNumberSize();
            size_t sizeExtension = std::stoi(sizePicExtension_str); // sizeExtention : size_t
            pos += sizeExtension;

            std::string nameAndExtension(buf, a, pos); //

            return nameAndExtension + resourceManager.getPic_str(pic_hash);
        }
        catch (const std::exception &e) {
            return "";
        }

    }
}