#pragma once

namespace pokemon {

    template<typename P>
    struct Data_to_send {
        std::string header{};
        P packet{};
    };

    class NETWORK_POKEMON_API command {
        public:

        template <typename T, typename P>
            static Data_to_send<P> prepare_data_to_send(const P& item) noexcept {
                Data_to_send<P> data;

                const P packet = T::to_packet(item);
                const size_t total_bytes = sizeof(P);
                const std::string header = Utils::formatted_number(total_bytes);
                data.header = header;
                data.packet = packet;
                return data;
            }

            template <typename T, typename P>
            static void send_list(const std::shared_ptr<tcp::IConnection>& socket, const std::vector<T>& items) noexcept{
                if (items.empty()) return;

                std::vector<P> packet_buffer;
                packet_buffer.reserve(items.size());

                for (const auto& item : items) {
                    const P packet = T::to_packet(item);
                    packet_buffer.push_back(packet);
                }

                const size_t total_bytes = packet_buffer.size() * sizeof(P);
                const std::string header = Utils::formatted_number(total_bytes);

                socket->write(header.data(), header.size());
                socket->write(reinterpret_cast<const char*>(packet_buffer.data()), total_bytes);
            }

            template <typename P>
            static void send_packets(const std::shared_ptr<tcp::IConnection>& socket, const std::vector<P>& packets) noexcept {
                if (packets.empty()) return;

                const size_t total_bytes = packets.size() * sizeof(P);
                const std::string header = Utils::formatted_number(total_bytes);

                socket->write(header.data(), header.size());
                socket->write(reinterpret_cast<const char*>(packets.data()), total_bytes);
            }

            template <typename T, typename P>
            static void send_item(const std::shared_ptr<tcp::IConnection>& socket, const T& item) noexcept {
                const P packet = T::to_packet(item);
                const size_t total_bytes = sizeof(P);
                const std::string header = Utils::formatted_number(total_bytes);

                socket->write(header.data(), header.size());
                socket->write(reinterpret_cast<const char*>(&packet), total_bytes);
            }

            template<typename P>
            static std::optional<std::vector<P>> receive_list(const std::shared_ptr<tcp::tcp_connector> &connector) {

                size_t total_bytes = 0;
                try {
                    total_bytes = Utils::get_total_bytes_from_connector(connector);
                }
                catch (...) {
                    std::cout << "Error when reading list from connector" << std::endl;
                    return std::nullopt;
                }

                if (total_bytes == 0 || total_bytes % sizeof(P) != 0) {
                    if (total_bytes == 0) return std::nullopt;
                    connector->shutdown();
                    return std::nullopt;
                }

                size_t count = total_bytes / sizeof(P);
                std::vector<P> packet_buffer(count);

                if (!Utils::read_exact(connector, reinterpret_cast<std::byte*>(packet_buffer.data()), total_bytes)) {
                    connector->shutdown();
                    return std::nullopt;
                }
                return packet_buffer;
            }


            template <typename P, typename T>
            static std::optional<P> receive_item(const std::shared_ptr<T> &conn) {
                try {
                    size_t total_bytes = Utils::get_total_bytes_from_connector(conn);
                    if (total_bytes != sizeof(P)) {
                        conn->shutdown();
                        return std::nullopt;
                    }

                    P packet;
                    if (!Utils::read_exact(conn, reinterpret_cast<std::byte*>(&packet), sizeof(P))) {
                        conn->shutdown();
                        return std::nullopt;
                    }
                    return packet;
                }
                catch (...) {
                    std::cout << "Error when reading item from connector" << std::endl;
                    conn->shutdown();
                    return std::nullopt;
                }
            }

            static std::string safe_string(const char* data, const size_t max_len) noexcept {
                size_t len = 0;
                while(len < max_len && data[len] != '\0') len++;
                return std::string(data, len);
            }
    };
}
