#pragma once

namespace pokemon {


    class NETWORK_POKEMON_API command {
        public:

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

            template<typename P>
            static std::optional<std::vector<P>> receive_list(const std::shared_ptr<tcp::tcp_connector> &connector) {

                size_t total_bytes = Utils::get_total_bytes_from_connector(connector);

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

            static std::string safe_string(const char* data, const size_t max_len) noexcept {
                size_t len = 0;
                while(len < max_len && data[len] != '\0') len++;
                return std::string(data, len);
            }
    };
}
