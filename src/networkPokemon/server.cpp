#include "pch.h"


using namespace std::chrono_literals;
namespace pokemon {

    Server::Server(in_port_t port, const std::shared_ptr<Node_Info> node_info) noexcept : NetworkNode(port, node_info){}

    /*std::thread Server::run(std::unique_ptr<sockpp::tcp_socket> socket) noexcept {
        return std::thread([this, socket = std::move(socket)]() mutable {
            process(std::move(socket));
        });
    }*/
#if 0
    int Server::process(std::unique_ptr<sockpp::tcp_socket> socket) {

        if (!socket || !(*socket)) {
            std::cout << "socket is null" << std::endl;
        }
        char buf[16];
        ssize_t n = socket->read(buf, sizeof(buf));
        std::cout << "received: " << buf << std::endl;
        std::cout << "rec " << std::endl;

        std::string std_send("cool");
        socket->write(&std_send[0], std_send.size());

        //while (socket.read(buf, sizeof(buf))) {
          //  std::cout << "received data: " << buf << std::endl;
        //}
    }
#endif

    int Server::process(std::shared_ptr<sockpp::tcp_socket> socket) {

        if (!socket || !(*socket)) {
           return -1;
        }

        std::cout << "server:: connection address: " << socket->address().to_string() << std::endl;
        char buf[SERVER_BUF_SIZE];
        std::string b;

        getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_CONNECTION, std::format(MSG_NODE_ID, getPort(), SERVER),
                                          socket->address().to_string(), getPort()));

        if (const auto res = socket->read(buf, sizeof(buf)); !res || res <= 0) {
            return -1;
        }


        std::string buf_str(buf, SERVER_BUF_SIZE);

        std::cout << "Server:: buffer: " << buf_str << std::endl;

        std::string protocol_str(buf, protocolSize()); // get protocole

        std::cout << "Server:: protocol: " << protocol_str << std::endl;

        getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_QUERY, std::format(MSG_NODE_ID, getPort(), SERVER),
                                              protocol_str));

        m_dispatcher.dispatch_write(*this, string_to_protocol(protocol_str), std::move(socket));
       // m_dispatcher.dispatch_write(*this, protocolToString(PROTOCOL::GET_IPS), std::move(socket));

        /*if (protocol_str == protocolToString(PROTOCOL::GET_IPS)) {
            return send_msg(std::move(socket), getIpsToSend(), protocol_str );
        }
        if (protocol_str == protocolToString(PROTOCOL::GET_PICS)) {
            return send_msg(std::move(socket), getPicsToSend(), protocol_str );
        }

        if (protocol_str == protocolToString(PROTOCOL::GET_ALIVE)) {
            return send_msg(std::move(socket), std::format("{};{}", "ALIVE", getPort() ), protocol_str );
        }

        if (protocol_str == protocolToString(PROTOCOL::GET_PIC)) {
            std::string std_send = getPicToSend(buf_str, buf);
            if(std_send.empty()){
                return -1;
            }
            return send_msg(std::move(socket), std_send, protocol_str );
        }*/
        return 0;
    }


    int Server::send_msg(std::shared_ptr<sockpp::tcp_socket> socket, const std::string_view &msg, const std::string_view &protocol ) const noexcept {

        if (!socket || !(*socket)) {
            return -1;
        }
        // Renvoie une chaine de format 00XX + receivedData + std_send
        // Pour pouvoir connaitre la taille de std_send pour creer mon buf cote client
        // Et pouvoir idendifier la requete
        const std::string std_send = std::format("{}{}{}", generateFormattedNumber(msg.size()), protocol, msg);

        socket->write(&std_send[0], std_send.size());

        if (protocol == protocolToString(PROTOCOL::GET_IPS)) {
            getTrace().print(std::clog, std::format(MSG_SERVER_SENT_IPS_LIST, std::format(MSG_NODE_ID, getPort(), SERVER),
                                              socket->address().to_string()));
        }

        else if (protocol == protocolToString(PROTOCOL::GET_PICS)) {
            getTrace().print(std::clog, std::format(MSG_SERVER_SENT_PICTURES_LIST, std::format(MSG_NODE_ID, getPort(), SERVER),
                                              socket->address().to_string()));
        }
        else if (protocol == protocolToString(PROTOCOL::GET_PIC)) {
            getTrace().print(std::clog, std::format(MSG_SERVER_SENT_PICTURE, std::format(MSG_NODE_ID, getPort(), SERVER),
                                              socket->address().to_string()));
        }

        socket->shutdown(SHUT_RDWR);
        return 0;
    }



#if 0
    int Server::process(sockpp::tcp_socket &socket) {
        std::string id_str("[node - " + std::to_string(getPort()) + "] - Server");

        size_t sizeBuf = 512;
        char buf[sizeBuf];
       /* sockpp::result<size_t> res;


        while ((res = socket.read(buf, sizeof(buf))) && res.value() > 0) {
            std::string socketAdress(socket.address().to_string());
            std::string std_send("");
            std::string buf_str(buf, sizeBuf);

            std::string protocoleStr(buf, protocolSize()); // get protocole
            trace.print(std::clog, id_str + " - received " + protocoleStr + " query");

            if (protocoleStr == protocolToString(PROTOCOL::GET_IPS)) {
                std_send = getIpsToSend();
                trace.print(std::clog, id_str + " - ports list was sent to " + socketAdress);
            }
            else if (protocoleStr == protocolToString(PROTOCOL::GET_PICS)) {
                std_send = getPicsToSend();
                trace.print(std::clog, id_str + " - Pictures list was sent to " + socketAdress);
            }
            else if (protocoleStr == protocolToString(PROTOCOL::GET_PIC)) {
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
        }*/
        memset(buf, 0, sizeBuf);
        socket.shutdown(SHUT_RDWR);
        return 0;
    }
#endif




    std::string Server:: getPicsToSend() const {
        std::string str("");
        for (auto &msg: getRessource().getPicturesList())
            str += msg.first + "," + std::get<0>(msg.second) + ","
                   + std::get<1>(msg.second) + ","
                   + std::get<2>(msg.second) + ";";
        return str;
    }

    std::string Server::getPicToSend(const std::string &buf_str, const char *buf) const{
        // sizePictureHash + pictureHash +  sizePictureName  + pictureName + sizeExtention  + extention
        std::string str("");
        size_t pos = protocolSize();

        std::string sizeHash_str = buf_str.substr(pos, FORMATTED_NUMBER_SIZE);  // sizePictureHash : str
        pos += FORMATTED_NUMBER_SIZE;
        getTrace().print(std::clog, sizeHash_str);
        size_t sizeHash;
        try {
            sizeHash = std::stoi(sizeHash_str); // sizePictureHash : size_t
            std::string pic_hash = buf_str.substr(pos, sizeHash); // pictureHash : str
            pos += sizeHash;
            getTrace().print(std::clog, pic_hash);

            size_t a = pos;
            std::string sizePicName_str = buf_str.substr(pos, FORMATTED_NUMBER_SIZE); // sizePictureName : str
            pos += FORMATTED_NUMBER_SIZE;
            size_t sizePicName = std::stoi(sizePicName_str); // sizePictureName : size_t
            pos += sizePicName;

            std::string sizePicExtension_str = buf_str.substr(pos, FORMATTED_NUMBER_SIZE); // sizeExtention : str
            pos += FORMATTED_NUMBER_SIZE;
            size_t sizeExtension = std::stoi(sizePicExtension_str); // sizeExtention : size_t
            pos += sizeExtension;

            std::string nameAndExtension(buf, a, pos); //

            return nameAndExtension + getRessource().getPic_str(pic_hash);
        }
        catch (const std::exception &e) {
            return "";
        }

    }
}
