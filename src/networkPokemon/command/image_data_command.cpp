#include "../pch.h"

namespace pokemon {


    void image_data_command::send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) {
        if (!socket) {
            return;
        }

       /* const std::string hash(hashBuf.begin(), hashBuf.end());
        const auto image = ss.get_images_repository().find_image(hash);
        if (!image.has_value()) {
            socket->shutdown();
            return;
        }

        const std::string data = ss.get_images_repository().get_picture_base64(image.value());

        const image_cache cache(hash, data);
        send_image(socket, cache);
        socket->shutdown(); */
    }

    void image_data_command::receive_from_server(Client& client, std::shared_ptr<tcp::tcp_connector> connector) {
        if (connector == nullptr || !(*connector)) {
            return;
        }
       // receive_nodes_list(client, connector);
        connector->shutdown();
    }
    void image_data_command::send_image(const std::shared_ptr<tcp::IConnection>& socket, const image_cache item) noexcept{
        command::send_item<image_cache, Image_Cache_Packet>(socket, item);
    }

    void image_data_command::receive_image(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector) {

        auto packet_buffer_opt = command::receive_list<Node_Packet>(connector);
        if (!packet_buffer_opt.has_value()) {
            return;
        }

        auto const& packet_buffer = packet_buffer_opt.value();
/*
        for (const auto& packet : packet_buffer) {

            std::string id = command::safe_string(packet.id, sizeof(packet.id));
            std::string name = command::safe_string(packet.name, sizeof(packet.name));
            std::string ip = command::safe_string(packet.ip, sizeof(packet.ip));

            // Big Endian -> Little Endian
            const auto port = ntohs(packet.port);

            if (ip.empty() || port == 0) continue;

            Node_Info node(id, name, ip, port);
            client.get_peer_registry().add_node(node);
            client.get_storage()->addNodeToSavedList(node);
        }*/
    }
    

}
