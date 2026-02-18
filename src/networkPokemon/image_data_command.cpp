#include "pch.h"

namespace pokemon {

    void image_data_command::send_to_client(session& ss, PROTOCOL protocol, std::shared_ptr<sockpp::tcp_socket> socket) {
        if (!socket || !(*socket)) {
            return;
        }


        char buf[FORMATTED_NUMBER_SIZE];
        std::string b;


        if (const auto res = socket->read(buf, sizeof(buf)); !res || res <= 0) {
            return;
        }


        std::string size_(buf, FORMATTED_NUMBER_SIZE);
        size_t length;
        try {
            length = std::stoi(size_); // Convert la taille du message
        }
        catch (const std::exception) {
            socket->shutdown(SHUT_RDWR);
            return;
        }

        auto read_exact = [&](char* buffer, size_t length) -> bool {
            size_t total_read = 0;
            while (total_read < length) {
                ssize_t n = socket->read(buffer + total_read, length - total_read);
                if (n <= 0) return false; // Erreur ou déconnexion
                total_read += n;
            }
            return true;
        };


        std::vector<char> hashBuf(length);
        if (!read_exact(hashBuf.data(), length)) {
            socket->shutdown(SHUT_RDWR);
            return;
        }

        std::string hash(hashBuf.begin(), hashBuf.end());
        auto image = ss.get_images_repository().find_image(hash);

        if (!image.has_value()) {
            socket->shutdown(SHUT_RDWR);
            return;
        }

        std::string data = ss.get_images_repository().get_picture_base64(image.value());


        std::string msg = std::format("{};{}", image->get_hash(),data);
        const std::string std_send = std::format("{}{}{}", ss.generateFormattedNumber(msg.size()), ss.protocolToString(protocol), msg);
        std::cout << std_send << std::endl;
        socket->write(&std_send[0], std_send.size());
        socket->shutdown(SHUT_RDWR);
    }

    void image_data_command::receive_from_server(Client& client, std::shared_ptr<sockpp::tcp_connector> connector) {
       /* std::stringstream ss(payload.data());
        std::string hash;
        std::string data;

        std::getline(ss, hash, ';');
        std::getline(ss, data);

        image_cache cache(hash, data);
        //client.get_images_repository().add(image);
        client.get_storage()->addImageCacheToSavedList(cache);*/

    }

}
