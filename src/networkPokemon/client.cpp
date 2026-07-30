#include "pch.h"

namespace pokemon {

    Client::Client(std::string_view ip, const in_port_t port, const std::shared_ptr<Node_Info> node_info,
        peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
            : NetworkNode(port, node_info, peers,images_repository, storage)
            ,  ip_s(ip) {
        initCommands();

        auto check_connected_thread = std::jthread([this] { check_connected_nodes(); });
        check_connected_thread.detach();

        auto get_ips_thread = std::jthread([this] { get_client_ip(); });
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

    void Client::add_new_node(std::string peer_ip, in_port_t port) noexcept {
        auto get_id = std::thread([this, peer_ip, port] { get_client_id(peer_ip, port); });
        get_id.detach();
    }

    void Client::get_picture(std::string hash) noexcept {
        {
            std::lock_guard<std::mutex> lock(m_pendingPicturesMutex);
            if (!m_pendingPictures.insert(hash).second) {
                return;
            }
        }

        auto get_pic = std::thread([this, hash] {
            get_client_picture(hash);
            std::lock_guard<std::mutex> lock(m_pendingPicturesMutex);
            m_pendingPictures.erase(hash);
        });
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

    void Client::get_client_id(std::string_view ip, in_port_t port) noexcept {
        const std::string msg = pokemon::tcp::protocolToString(tcp::PROTOCOL::GET_ID);
        auto task = [this, ip = ip, port = port, msg]() {
            this->start(ip, port, msg);
        };
       std::thread t(task);
       t.join();
    }


    void Client::get_client_picture(std::string hash) noexcept {

        auto image = get_images_repository().find_image(hash);
        if (!image.has_value()) {
            std::cout << "[GET_PIC][client] hash=" << hash << " introuvable dans le catalogue local (jamais recu via GET_PICS ?)" << std::endl;
            return;
        }
        try{
            auto nodeInfo = get_peer_registry().find_node_by_id(image.value().get_owner());
            std::cout << "[GET_PIC][client] demande hash=" << hash << " au proprietaire "
                      << nodeInfo.get_ip() << ":" << nodeInfo.get_port() << std::endl;

            const Image_Packet packet = Image::to_packet(image.value());
            const size_t total_bytes = sizeof(Image_Packet);

            auto pk = reinterpret_cast<const char*>(&packet);

            const std::string header = Utils::formatted_number(total_bytes);
            std::string msg = std::format("{}{}{}", tcp::protocolToString(tcp::PROTOCOL::GET_PIC),header, std::string(pk, total_bytes));

            auto task = [this, ip = nodeInfo.get_ip(), port = nodeInfo.get_port(), msg]() {
                this->start(ip, static_cast<in_port_t>(port), msg);
            };
            std::thread t(task);
            t.join();

        }
        catch (const std::exception& e) {
            std::cout << "[GET_PIC][client] echec pour hash=" << hash << " : propriétaire introuvable dans le registre de pairs (" << e.what() << ")" << std::endl;
        }
    }


    void Client::get_client_ip() noexcept {
        while (true) {
            for (auto &node: get_peer_registry().get_nodes()) {
                const std::string msg = tcp::protocolToString(tcp::PROTOCOL::GET_IPS);

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
                std::string_view msg = tcp::protocolToString(tcp::PROTOCOL::GET_PICS);

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
                const std::string msg = tcp::protocolToString(tcp::PROTOCOL::GET_ALIVE);
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


    int Client::start(std::string_view neighbour_ip, in_port_t neighbour_port, std::string_view msg) noexcept {
        tcp::client_net<Client> net;
        return net.client_ask_to_server(*this, get_dispatcher(), neighbour_ip, neighbour_port, msg);
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



    std::shared_ptr<Image> Client::add_pokemon(std::string_view name, std::string_view picturePath) noexcept {
        if (get_node_info() == nullptr)
            return nullptr;
        return  get_images_repository().add_picture_from_path(name, get_node_info()->get_id(), picturePath);

    }

}
