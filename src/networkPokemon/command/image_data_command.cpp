#include "../pch.h"

namespace pokemon {


    void image_data_command::send_to_client(session& ss, const std::shared_ptr<tcp::IConnection>& socket) {
        if (!socket) {
            return;
        }

        auto packet_opt = command::receive_item<Image_Packet>(socket);
        if (!packet_opt.has_value()) {
            std::cout << "[GET_PIC][serveur] requete illisible (en-tete/paquet invalide)" << std::endl;
            socket->shutdown();
            return;
        }

        auto image = Image::from_packet(packet_opt.value());
        std::cout << "[GET_PIC][serveur] requete pour hash=" << image.get_hash() << std::endl;

        const std::string data = ss.get_images_repository().get_picture_base64(image);
        std::cout << "[GET_PIC][serveur] taille des donnees renvoyees=" << data.size()
                   << (data.empty() ? " (VIDE : fichier introuvable sur le disque du proprietaire)" : "") << std::endl;

        const image_cache cache(image.get_hash(), data);
        send_image(socket, cache);
        socket->shutdown();

    }

    void image_data_command::receive_from_server(Client& client, std::shared_ptr<tcp::tcp_connector> connector) {
        if (!(*connector) || connector == nullptr ) {
            std::cout << " Connector is null, can't get pictures from the server ... " << std::endl;
            return;
        }
        receive_image(client, connector);
        connector->shutdown();
    }
    void image_data_command::send_image(const std::shared_ptr<tcp::IConnection>& socket, const image_cache item) noexcept{
        const auto packets = image_cache::to_packets(item);
        command::send_packets<Image_Cache_Packet>(socket, packets);
    }

    void image_data_command::receive_image(const Client& client, const std::shared_ptr<tcp::tcp_connector> &connector) {

        auto packets_opt = command::receive_list<Image_Cache_Packet>(connector);
        if (!packets_opt.has_value()) {
            std::cout << "[GET_PIC][client] aucun paquet recu (echec de lecture reseau)" << std::endl;
            return;
        }
        std::cout << "[GET_PIC][client] " << packets_opt->size() << " paquet(s) recu(s)" << std::endl;

        auto cache_opt = image_cache::from_packets(packets_opt.value());
        if (!cache_opt.has_value()) {
            std::cout << "[GET_PIC][client] image recue incomplete ou corrompue (chunks manquants/invalides)" << std::endl;
            return;
        }

        const image_cache& cache = cache_opt.value();
        std::cout << "[GET_PIC][client] reassemblee : hash=" << cache.get_hash()
                   << " taille=" << cache.get_data().size() << " octets" << std::endl;

        if (!image_repository::verify_base64_sha256(cache.get_hash(), cache.get_data())) {
            std::cout << "[GET_PIC][client] REJETEE : le hash SHA-256 ne correspond pas aux donnees (hash="
                       << cache.get_hash() << ")" << std::endl;
            return;
        }

        std::cout << "[GET_PIC][client] hash verifie OK, mise en cache (hash=" << cache.get_hash() << ")" << std::endl;
        client.get_storage()->addImageCacheToSavedList(cache);
    }


}
