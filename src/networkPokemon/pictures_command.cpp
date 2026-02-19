#include "pch.h"

namespace pokemon {

    std::string pictures_command::get_pictures_to_send(const std::vector<Image> images) const {
        std::string str;
        for (const auto &image: images)
            str += std::format("{}_{}_{}_{}_{}_{};", image.get_name(),
                image.get_extension(), image.get_hash(), image.get_owner(), image.get_size(), image.get_size_unit());
        return str;
    }

    void pictures_command::send_to_client(session& ss, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }

        std::string msg = get_pictures_to_send(ss.get_images_repository().get_images());
        const std::string std_send = std::format("{}{}{}", ss.generateFormattedNumber(msg.size()), ss.protocolToString(PROTOCOL::GET_PICS), msg);
        std::cout << std_send << std::endl;
        socket->write(&std_send[0], std_send.size());
        socket->shutdown(SHUT_RDWR);
    }

    void pictures_command::receive_from_server([[maybe_unused]] Client& client, [[maybe_unused]] std::shared_ptr<sockpp::tcp_connector> connector) {
       /* std::stringstream ss(payload.data());
        std::string data;

        while (std::getline(ss, data, ';')) {
            std::string name_str, extension_str, hash_str, owner_str, size_str, size_unit_str;
            std::stringstream ss2(data);

            std::getline(ss2, name_str, '_');
            std::getline(ss2, extension_str, '_');
            std::getline(ss2, hash_str, '_');
            std::getline(ss2, owner_str, '_');
            std::getline(ss2, size_str, '_');
            std::getline(ss2, size_unit_str);

            try {
                Image image(name_str, extension_str, hash_str, owner_str, size_str, size_unit_str);
                client.get_images_repository().add_image(image);
                client.get_storage()->addImageToSavedList(image);
            } catch (...) {
            }
        }*/
    }
}
