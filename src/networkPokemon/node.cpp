#include "pch.h"
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <iphlpapi.h>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <ifaddrs.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#endif

using namespace std::chrono_literals;

namespace {
#ifdef _WIN32
    using socket_handle_t = SOCKET;
    constexpr socket_handle_t k_invalid_socket = INVALID_SOCKET;
    inline void close_socket_handle(socket_handle_t s) noexcept { closesocket(s); }
#else
    using socket_handle_t = int;
    constexpr socket_handle_t k_invalid_socket = -1;
    inline void close_socket_handle(socket_handle_t s) noexcept { close(s); }
#endif

    // Owns a raw probing socket and guarantees it is closed on every exit path
    // (including exceptions), so find_available_port never leaks a descriptor.
    class raw_socket_guard {
    public:
        explicit raw_socket_guard(socket_handle_t handle) noexcept : handle_(handle) {}
        raw_socket_guard(const raw_socket_guard&) = delete;
        raw_socket_guard& operator=(const raw_socket_guard&) = delete;
        raw_socket_guard(raw_socket_guard&&) = delete;
        raw_socket_guard& operator=(raw_socket_guard&&) = delete;

        ~raw_socket_guard() {
            if (handle_ != k_invalid_socket) {
                close_socket_handle(handle_);
            }
        }

        [[nodiscard]] bool valid() const noexcept { return handle_ != k_invalid_socket; }
        [[nodiscard]] socket_handle_t get() const noexcept { return handle_; }

    private:
        socket_handle_t handle_;
    };
}

namespace pokemon {
    Node::Node(peer_registry& peers, image_repository& image_repository) noexcept
        : peers_(peers)
        , image_repository_(image_repository)
    {}

    void Node::initialized() noexcept{
        m_storage = std::make_shared<storage_manager>(image_repository_.get_storage_path());

        auto node_info_data = m_storage->loadNodeInfo();
        if (node_info_data.has_value()) {
            m_node_info = std::make_shared<Node_Info>(node_info_data.value());
            m_node_info->set_ip(get_network_ip());
        }
        else {
            m_node_info = std::make_shared<Node_Info>(Utils::generate_uuid_v4(), Node_Info::DEFAULT_NODE_NAME, get_network_ip(), find_available_port(DEFAULT_PREFERRED_PORT));
        }

        m_storage->saveNodeInfo(*m_node_info);
        m_storage->load_nodes(peers_);
        m_storage->load_images(image_repository_);

      //  sockpp::initialize();

        m_server = std::make_unique<Server>(m_node_info->get_port(), m_node_info, peers_, image_repository_, m_storage);
        client = std::make_unique<Client>(m_node_info->get_ip(), m_node_info->get_port(), m_node_info, peers_, image_repository_, m_storage);
    }

    std::string Node::get_picture(const Image image) {
        std::string rawData;
        if (image.get_owner() == m_node_info->get_id()) {
            rawData = image_repository_.get_picture_base64(image);
        }
        else {
            rawData = m_storage->get_image_cache_data(image.get_hash());
            if (rawData.empty()) {
                client->get_picture(image.get_hash().data());
                rawData = m_storage->get_image_cache_data(image.get_hash());
            }
        }

        if (rawData.empty()) {
            return "";
        }

        return rawData;
    }


     void Node::set_node_info(std::string_view node_name, int port, int max_connections, bool auto_share, bool auto_download) noexcept {
        if (m_node_info == nullptr)
            m_node_info =  std::make_shared<Node_Info>();

        m_node_info->set_name(node_name);
        if (port > 0) {
            // Le serveur déjà lancé écoute toujours sur l'ancien port : ce
            // changement ne prend effet qu'au prochain démarrage du nœud.
            m_node_info->set_port(port);
        }
        m_node_info->set_max_connections(max_connections);
        m_node_info->set_auto_share(auto_share);
        m_node_info->set_auto_download(auto_download);

        if (m_storage) {
            m_storage->saveNodeInfo(*m_node_info);
        }
    }

  std::string Node::get_network_ip() const {
        std::string ipAddress = LOCALHOST_IP.data();

#ifdef _WIN32
        ULONG outBufLen = 15000;
        PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);
        if (pAddresses == nullptr) return ipAddress;

        ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER;

        if (GetAdaptersAddresses(AF_INET, flags, NULL, pAddresses, &outBufLen) == NO_ERROR) {
            PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
            while (pCurrAddresses) {
                if (pCurrAddresses->IfType != IF_TYPE_SOFTWARE_LOOPBACK && pCurrAddresses->FirstUnicastAddress != nullptr) {

                    struct sockaddr_in* sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in*>(pCurrAddresses->FirstUnicastAddress->Address.lpSockaddr);
                    char addressBuffer[INET_ADDRSTRLEN];
                    inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), addressBuffer, INET_ADDRSTRLEN);

                    ipAddress = addressBuffer;

                    break;
                }
                pCurrAddresses = pCurrAddresses->Next;
            }
        }
        free(pAddresses);

#else
        struct ifaddrs *interfaces = nullptr;
        struct ifaddrs *temp_addr = nullptr;
        int success = 0;

        success = getifaddrs(&interfaces);

        if (success == 0) {
            temp_addr = interfaces;
            while (temp_addr != nullptr) {
                if (temp_addr->ifa_addr != nullptr && temp_addr->ifa_addr->sa_family == AF_INET) {
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
        if (interfaces) {
            freeifaddrs(interfaces);
        }
#endif

        return ipAddress;
    }

    in_port_t Node::find_available_port(in_port_t preferred_port) {
        const socket_handle_t sock = socket(AF_INET, SOCK_STREAM, 0);
        raw_socket_guard guard(sock);
        if (!guard.valid()) {
            return preferred_port;
        }

        struct sockaddr_in sin{};
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = INADDR_ANY;
        sin.sin_port = htons(preferred_port);

        if (bind(guard.get(), reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) == 0) {
            return preferred_port;
        }

        // Le port préféré est occupé : laisse l'OS en choisir un libre.
        sin.sin_port = 0;
        if (bind(guard.get(), reinterpret_cast<struct sockaddr*>(&sin), sizeof(sin)) != 0) {
            return preferred_port;
        }

        socklen_t len = sizeof(sin);
        if (getsockname(guard.get(), reinterpret_cast<struct sockaddr*>(&sin), &len) != 0) {
            return preferred_port;
        }

        return ntohs(sin.sin_port);
    }

    void Node::add_new_peer(std::string peer_ip) noexcept {

        add_peer(peer_ip, DEFAULT_PREFERRED_PORT);
    }

    void Node::add_peer(std::string peer_ip, in_port_t port) noexcept {
        client->add_new_node(peer_ip, port);
    }


    std::ostream &operator<<(std::ostream &os, const Node &node) {
        os << "[ Node " << " ] : " << "Ip: " << node.m_node_info->get_ip() << " port: " << node.m_node_info->get_port() << std::endl;
        auto nodesList = node.peers_.get_nodes();
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

    void Node::remove_pokemon(std::string_view hash) noexcept {
        image_repository_.remove_image(hash);
        if (m_storage) {
            m_storage->removeImageFromSavedList(hash);
            m_storage->removeImageCacheEntry(hash);
        }
    }
}
