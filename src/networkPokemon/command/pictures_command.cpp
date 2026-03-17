#include "../pch.h"

namespace pokemon {

    void pictures_command::send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) {
        if (!socket) {
            return;
        }
        send_pictures_list(socket, ss.get_images_repository().get_images());
        socket->shutdown();
    }

    void pictures_command::receive_from_server(Client& client, std::shared_ptr<tcp::tcp_connector> connector) {
        if (connector == nullptr || !(*connector)) {
            return;
        }
        receive_pictures_list(client, connector);
        connector->shutdown();
    }



    void pictures_command::send_pictures_list(const std::shared_ptr<tcp::IConnection>& socket, const std::vector<Image>& images) noexcept {
        command::send_list<Image, Image_Packet>(socket, images);
    }
     void pictures_command::receive_pictures_list(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector) {
        auto packet_buffer_opt = command::receive_list<Image_Packet>(connector);
        if (!packet_buffer_opt.has_value()) {
            return;
        }

        auto const& packet_buffer = packet_buffer_opt.value();

        for (const auto& packet : packet_buffer) {

            std::string name = command::safe_string(packet.name, sizeof(packet.name));
            std::string extension = command::safe_string(packet.extension, sizeof(packet.extension));
            std::string hash = command::safe_string(packet.hash, sizeof(packet.hash));
            std::string owner = command::safe_string(packet.owner, sizeof(packet.owner));
            std::string size = command::safe_string(packet.size, sizeof(packet.size));
            std::string size_unit = command::safe_string(packet.size_unit, sizeof(packet.size_unit));

            if (name.empty() || hash.empty()) continue;

            Image image(name, extension, hash, owner, size, size_unit);
            client.get_images_repository().add_image(image);
        }
    }
}
