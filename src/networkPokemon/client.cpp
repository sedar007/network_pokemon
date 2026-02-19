#include "pch.h"

namespace pokemon {

    Client::Client(std::string_view ip, const in_port_t port, const std::shared_ptr<Node_Info> node_info,
        peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
            : NetworkNode(port, node_info, peers,images_repository, storage)
            ,  ip_s(ip) {
      //  auto run = getIps();
      //  run.detach();

        auto check_connected_thread = std::thread([this] { check_connected_nodes(); });
        check_connected_thread.detach();

        auto get_ips_thread = std::thread([this] { get_client_ip(); });
        get_ips_thread.detach();

        auto get_pictures_thread = std::thread([this] { get_client_pictures(); });
        get_pictures_thread.detach();


    }




    Client::~Client() {
        m_running = false;

        std::lock_guard<std::mutex> lock(m_thread_mutex);
        for (auto& t : m_threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

   /* std::thread Client::run(std::string_view neighbour_ip, const in_port_t neighbour_port, const std::string &msg) noexcept {
        return std::thread([this, neighbour_ip, neighbour_port, &msg] { start(neighbour_ip, neighbour_port, msg); });
    }*/


    void Client::add_new_node(std::string peer_ip, in_port_t port) noexcept {
        auto get_id = std::thread([this, peer_ip, port] { get_client_id(peer_ip, port); });
        get_id.detach();
    }

    void Client::get_picture(std::string hash) noexcept {
        auto get_pic = std::thread([this, hash] { get_client_picture(hash); });
        get_pic.detach();
    }


    std::thread Client::run(std::string_view neighbour_ip, const in_port_t neighbour_port, const std::string &msg) noexcept {
        std::thread t([this, neighbour_ip, neighbour_port, msg] {
            start(neighbour_ip, neighbour_port, msg);
        });

        {
            std::lock_guard<std::mutex> lock(m_thread_mutex);
            m_threads.push_back(std::move(t));
        }
        return std::thread();
    }

    void Client::run_getIp() noexcept {
        bool getIp = true;
        while (true) {
            std::string msg;
            /*for (auto &node: getRessource().getNodesInfoList()) {

                if (getIp)
                    msg = protocolToString(PROTOCOL::GET_IPS);
                else
                    msg = protocolToString(PROTOCOL::GET_PICS);

                auto task = [this, ip = node.get_ip(), port = node.get_port(), msg]() {
                    this->start(ip, port, msg);
                };

                auto check_connected_task = [this, ip = node.get_ip(), port = node.get_port()]() {
                    this->check_connected(ip, port);
                };
                enqueue_thread(check_connected_task);
                //enqueue_thread(task);

                std::this_thread::sleep_for(threadSleep_s(1500, 3500));
            }*/

           /*  for (auto &node: getRessource().getNodesList()) {
                std::string neighbour_ip;
                in_port_t neighbour_port;
                if(getPort_Ip(node, neighbour_ip, neighbour_port) == -1)
                    continue;
                if (getIp)
                    msg = protocolToString(PROTOCOL::GET_IPS);
               else
                    msg = protocolToString(PROTOCOL::GET_PICS);

                auto task = [this, ip = neighbour_ip, port = neighbour_port, msg]() {
                    this->start(ip, port, msg);
                };

                enqueue_thread(task);
                std::this_thread::sleep_for(threadSleep_s(500, 1500));

            }*/
            getIp = !getIp;
            std::this_thread::sleep_for(threadSleep_s(5000, 7000));
        }
    }

    void Client::get_client_id(std::string_view ip, in_port_t port) noexcept {

        std::string_view msg = protocolToString(PROTOCOL::GET_ID);

        auto task = [this, ip = ip, port = port, msg]() {
            this->start(ip, port, msg);
        };
       std::thread t(task);
       t.join();
    }


    void Client::get_client_picture(std::string hash) noexcept {

        auto image = get_images_repository().find_image(hash);
        if (!image.has_value()) {
            getTrace().print(std::cerr, "No image found with hash: " + std::string(hash));
            return;
        }
        try{
            auto nodeInfo = get_peer_registry().find_node_by_id(image.value().get_owner());

            std::string msg = std::format("{}{}{}", protocolToString(PROTOCOL::GET_PIC), generateFormattedNumber(hash.size()), hash);

            auto task = [this, ip = nodeInfo.get_ip(), port = nodeInfo.get_port(), msg]() {
                this->start(ip, static_cast<in_port_t>(port), msg);
            };
            std::thread t(task);
            t.join();

        }
        catch (const std::exception& e) {
            getTrace().print(std::cerr, "Error getting picture: " + std::string(e.what()));
        }
    }


    void Client::get_client_ip() noexcept {
        while (true) {
            for (auto &node: get_peer_registry().get_nodes()) {
                std::string_view msg = protocolToString(PROTOCOL::GET_IPS);

                auto task = [this, ip = node.get_ip(), port = node.get_port(), msg]() {
                    this->start(ip, static_cast<in_port_t>(port), msg);
                };
                enqueue_thread(task);
                std::this_thread::sleep_for(threadSleep_seconde(std::chrono::seconds(2), std::chrono::seconds(3)));
            }
            std::this_thread::sleep_for(threadSleep_seconde(std::chrono::seconds(2), std::chrono::seconds(5)));
        }
    }

    void Client::get_client_pictures() noexcept {
        while (true) {
            for (auto &node: get_peer_registry().get_nodes()) {
                std::string_view msg = protocolToString(PROTOCOL::GET_PICS);

                auto task = [this, ip = node.get_ip(), port = node.get_port(), msg]() {
                    this->start(ip, static_cast<in_port_t>(port), msg);
                };
                enqueue_thread(task);
                std::this_thread::sleep_for(threadSleep_seconde(std::chrono::seconds(2), std::chrono::seconds(3)));
            }
            std::this_thread::sleep_for(threadSleep_seconde(std::chrono::seconds(2), std::chrono::seconds(5)));
        }
    }



    void Client::check_connected_nodes() noexcept {
        while (true) {
            for (auto &node: get_peer_registry().get_nodes()) {
                std::string_view msg = protocolToString(PROTOCOL::GET_ALIVE);
                get_peer_registry().set_node_alive(node.get_ip(), node.get_port(), false);

                auto task = [this, ip = node.get_ip(), port = node.get_port(), msg]() {
                    this->start(ip, static_cast<in_port_t>(port), msg);
                };
                enqueue_thread(task);
                std::this_thread::sleep_for(threadSleep_seconde(std::chrono::seconds(2), std::chrono::seconds(3)));
            }
            std::this_thread::sleep_for(threadSleep_seconde(std::chrono::seconds(2), std::chrono::seconds(5)));
        }
    }

    std::thread Client::getIps() noexcept {
        return std::thread([this] { run_getIp(); });
    }

    int Client::start(std::string_view neighbour_ip, in_port_t neighbour_port, std::string_view msg) noexcept {
    try {

        std::string knowPortStr = std::to_string(neighbour_port);
        std::string ip_str(neighbour_ip);
        auto connector = std::make_shared<sockpp::tcp_connector>();

        getTrace().print(std::clog, std::format(MSG_CLIENT_TRYING_TO_CONNECT,
                                    std::format(MSG_NODE_ID, getPort(), CLIENT), knowPortStr));


        if (!connector->connect(sockpp::inet_address(ip_str, neighbour_port))) {
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONNECTING,
                                        std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));
            return -1;
        }

        getTrace().print(std::clog, std::format(MSG_CLIENT_CONNECTED,
                                    std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));


        if (auto res = connector->write(msg.data(), msg.size()); res != msg.size()) {
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_WRITING_TCP_STREAM,
                                        std::format(MSG_NODE_ID, getPort(), CLIENT), connector->last_error_str()));
            connector->shutdown(SHUT_RDWR);
            return 1;
        }

        PROTOCOL protocol = string_to_protocol(msg);

        if (protocol == PROTOCOL::UNKNOWN) {
            connector->shutdown(SHUT_RDWR);
            return 1;
        }

        m_dispatcher.dispatch_client_read(*this, protocol , connector);

        connector->shutdown(SHUT_RDWR);
        return 0;

    } catch (const std::exception& e) {
        getTrace().print(std::cerr, std::format("Exception in Client::start: {}", e.what()));
        return -1;
    }
}
#if 0
   /* int Client::start(std::string_view neighbour_ip, in_port_t neighbour_port, std::string_view msg) noexcept {

        try {


        if (ip_s == neighbour_ip) {
            return -1;
        }

        std::string ip_str(neighbour_ip);
        std::string knowPortStr(std::to_string(neighbour_port));

        sockpp::tcp_connector connector;

        getTrace().print(std::clog, std::format(MSG_CLIENT_TRYING_TO_CONNECT, std::format(MSG_NODE_ID, getPort(), CLIENT), knowPortStr));

        std::this_thread::sleep_for(threadSleep_s(1500, 3000));

        getRessource().set_node_a_live(ip_str, neighbour_port, false);

        if (!connector.connect(sockpp::inet_address(ip_str, neighbour_port))){
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONNECTING, std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));
            connector.shutdown(SHUT_RDWR);
        }

        getTrace().print(std::clog, std::format(MSG_CLIENT_CONNECTED, std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));

        getRessource().set_node_a_live(ip_str, neighbour_port, true);
        // Send
        if (auto res = connector.write(msg.data()); res != msg.size()) {
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_WRITING_TCP_STREAM, std::format(MSG_NODE_ID, getPort(), CLIENT), connector.last_error()));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }


        // read
        char sizeMsg[FORMATTED_NUMBER_SIZE]; // Recupere la taille du buffer
       /* if (auto res = connector.read(sizeMsg, FORMATTED_NUMBER_SIZE); !res) {
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM, std::format(MSG_NODE_ID, getPort(), CLIENT), connector.last_error()));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }*/
            if (!read_exact(connector, sizeMsg, FORMATTED_NUMBER_SIZE)) {
    getTrace().print(std::cerr, "Erreur lecture taille");
    connector.shutdown(SHUT_RDWR);
    return 1;
}


        // Le type de requete
        char queryBuf[protocolSize()]; // Recupere le protocol
       /* if (auto res = connector.read(queryBuf, protocolSize()); !res) {
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM, std::format(MSG_NODE_ID, getPort(), CLIENT), connector.last_error()));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }*/

            if (!read_exact(connector, queryBuf, protocolSize())) {
                getTrace().print(std::cerr, "Erreur lecture protocole");
                connector.shutdown(SHUT_RDWR);
                return 1;
            }

        std::string protocole(queryBuf, protocolSize());
        memset(queryBuf,0, protocolSize());

        size_t t;
        try {
            t = std::stoi(sizeMsg); // Convert la taille du message
        }
        catch (const std::exception) {
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONVERTING_SIZE, std::format(MSG_NODE_ID, getPort(), CLIENT), sizeMsg));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }

        /*char msg_buf[t];

        if (auto res = connector.read(msg_buf, t); !res) { // Recupere le message envoyer
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM, std::format(MSG_NODE_ID, getPort(), CLIENT), connector.last_error()));
            connector.shutdown(SHUT_RDWR);
            return 1;
        }*/
            std::vector<char> msg_buf(t);
if (!read_exact(connector, msg_buf.data(), t)) {
    getTrace().print(std::cerr, "Erreur lecture message");
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

       // getTrace().print(std::clog, nodeIdStr + " - Connection Closed from " + std::to_string(neighbour_port));
        connector.shutdown(SHUT_RDWR);
        return (!connector) ? -1 : 0;

        }
        catch (std::exception) {
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONNECTING, std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));
            return -1;
        }

    }*/
#endif

    int Client::check_connected(std::string_view neighbour_ip, in_port_t neighbour_port) noexcept {
            try {

            std::string ip_str(neighbour_ip);

            std::string knowPortStr = std::to_string(neighbour_port);

            sockpp::tcp_connector connector;

            // ---------------------------------------------------------
            // 1. Connexion
            // ---------------------------------------------------------
            getTrace().print(std::clog, std::format(MSG_CLIENT_TRYING_TO_CONNECT,
                                        std::format(MSG_NODE_ID, getPort(), CLIENT), knowPortStr));

            // Simulation de latence (si nécessaire)
            std::this_thread::sleep_for(threadSleep_s(1500, 3000));


            if (!connector.connect(sockpp::inet_address(ip_str, neighbour_port))) {
                getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONNECTING,
                                            std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));
                return -1; // Pas besoin de shutdown si connect a échoué
            }

            getTrace().print(std::clog, std::format(MSG_CLIENT_CONNECTED,
                                        std::format(MSG_NODE_ID, getPort(), CLIENT), neighbour_ip, neighbour_port));


            // ---------------------------------------------------------
            // 2. Écriture (Envoi de la requête)
            // ---------------------------------------------------------

            std::string msg = "Are you alive?";
            if (auto res = connector.write(msg.data(), msg.size()); res != msg.size()) {
                getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_WRITING_TCP_STREAM,
                                            std::format(MSG_NODE_ID, getPort(), CLIENT), connector.last_error_str()));
                connector.shutdown(SHUT_RDWR);
                return 1;
            }

            auto read_exact = [&](char* buffer, size_t length) -> bool {
                size_t total_read = 0;
                while (total_read < length) {
                    ssize_t n = connector.read(buffer + total_read, length - total_read);
                    if (n <= 0) return false; // Erreur ou déconnexion
                    total_read += n;
                }
                return true;
            };

            char sizeMsg[FORMATTED_NUMBER_SIZE];
            if (!read_exact(sizeMsg, FORMATTED_NUMBER_SIZE)) {
                getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM,
                                            std::format(MSG_NODE_ID, getPort(), CLIENT), "Read Size Failed"));
                connector.shutdown(SHUT_RDWR);
                return 1;
            }

            // B. Lecture du protocole
            size_t pSize = protocolSize();
            // Utilisation d'un vector pour garantir la sécurité mémoire, ou buffer fixe si pSize est constant petite
            std::vector<char> protocolBuf(pSize);
            if (!read_exact(protocolBuf.data(), pSize)) {
                getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM,
                                            std::format(MSG_NODE_ID, getPort(), CLIENT), "Read Protocol Failed"));
                connector.shutdown(SHUT_RDWR);
                return 1;
            }
            std::string protocole(protocolBuf.begin(), protocolBuf.end());

            // C. Conversion de la taille
            size_t t = 0;
            try {
                // std::string(sizeMsg, len) est important car sizeMsg n'est pas forcément null-terminated
                t = std::stoi(std::string(sizeMsg, FORMATTED_NUMBER_SIZE));
            }
            catch (const std::exception& e) {
                getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONVERTING_SIZE,
                                            std::format(MSG_NODE_ID, getPort(), CLIENT), std::string(sizeMsg, FORMATTED_NUMBER_SIZE)));
                connector.shutdown(SHUT_RDWR);
                return 1;
            }

            // D. Lecture du corps du message
            // Utilisation de vector au lieu de VLA (char msg_buf[t]) pour éviter Stack Overflow
            std::vector<char> msg_buf(t);
            if (!read_exact(msg_buf.data(), t)) {
                getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_READING_TCP_STREAM,
                                            std::format(MSG_NODE_ID, getPort(), CLIENT), "Read Body Failed"));
                connector.shutdown(SHUT_RDWR);
                return 1;
            }

            std::string msg_str(msg_buf.begin(), msg_buf.end());

            // ---------------------------------------------------------
            // 4. Traitement
            // ---------------------------------------------------------
            if (protocole == protocolToString(PROTOCOL::GET_IPS)) {
                addIps(msg_str);
            }
            else if (protocole == protocolToString(PROTOCOL::GET_PICS)) {

            }
            else if (protocole == protocolToString(PROTOCOL::GET_PIC)) {
                addPicture(msg_str);
            }

            // Fermeture propre
            connector.shutdown(SHUT_RDWR);
            return 0;

        } catch (const std::exception& e) {
            // Catch-all pour éviter que le thread ne fasse crasher l'appli entière
            getTrace().print(std::cerr, std::format("Exception in Client::start: {}", e.what()));
            return -1;
        }
    }


void Client::addIps(const std::string &ips_str) const noexcept {
    std::stringstream ss(ips_str);
    std::string data;
    while (std::getline(ss, data, ';')) {
        std::string id_str;
        std::string name_str;
        std::string ip_str;
        std::string port_str;

        std::stringstream ss2(data);

        std::getline(ss2, id_str, '_');
        std::getline(ss2, name_str, '_');
        std::getline(ss2, ip_str, '_');
        std::getline(ss2, port_str);

        if (!isValidIPAddress(ip_str)) {
            getTrace().print(std::clog, std::format(MSG_CLIENT_ERROR_INVALID_IP,
                std::format(MSG_NODE_ID, getPort(), CLIENT), ip_str));
            continue;
        }
        Node_Info nodeInfo(id_str, name_str, ip_str, std::stoi(port_str));

        /*if (nodeInfo == this)
            continue;*/
        get_peer_registry().add_node(nodeInfo);

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
        catch (const std::exception){
              std::cout<<SAVE_PICTURE_FAIL<<std::endl;
            getTrace().print(std::cerr, std::format(MSG_CLIENT_ERROR_CONVERTING_SIZE, std::format(MSG_NODE_ID, getPort(), CLIENT), name_size_buf));
            return;
        }

        std::string picName(str, pos, t);
        pos += t;
        std::string extension_size(str, pos, FORMATTED_NUMBER_SIZE);
        pos += FORMATTED_NUMBER_SIZE;
        try {
            t = std::stoi(extension_size);
        }
        catch (const std::exception){
            std::cout<<SAVE_PICTURE_FAIL<<std::endl;
            //getTrace().print(std::cerr, nodeIdStr + " - Error can't convert " + extension_size + " to a integer");
        }

        std::string extention(str, pos, t);
        pos += t;
        t = str.size() - pos;
        std::string pic_str(str, pos, t);

        if(getRessource().savedPictureToDisk(FOLDER_TO_SAVE , picName , extention, pic_str) == 0)
            std::cout<< SAVE_PICTURE_SUCCESS<<std::endl;
        else
            std::cout<< SAVE_PICTURE_FAIL<<std::endl;
    }



    std::shared_ptr<Image> Client::add_pokemon(std::string_view name, std::string_view picturePath) noexcept {
        if (get_node_info() == nullptr)
            return nullptr;
        return  get_images_repository().add_picture_from_path(name, get_node_info()->get_id(), picturePath);

    }

}
