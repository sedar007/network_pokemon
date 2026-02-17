#include "pch.h"


using namespace std::chrono_literals;
namespace pokemon {

    session::session(in_port_t port, const std::shared_ptr<Node_Info> node_info,
        peer_registry& peers, image_repository& images_repository, std::shared_ptr<storage_manager> storage) noexcept
        : NetworkNode(port, node_info, peers, images_repository, storage){}


    int session::process(std::shared_ptr<sockpp::tcp_socket> socket) {

        if (!socket || !(*socket)) {
           return -1;
        }

        char protocol_buf[PROTOCOL_SIZE];

        getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_CONNECTION, std::format(MSG_NODE_ID, getPort(), SERVER),
                                          socket->address().to_string(), getPort()));

        if (const auto res = socket->read(protocol_buf, sizeof(protocol_buf)); !res || res <= 0) {
            return -1;
        }

        std::string protocol_str(protocol_buf, PROTOCOL_SIZE);
        getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_QUERY, std::format(MSG_NODE_ID, getPort(), SERVER),
                                              protocol_str));

        m_dispatcher.dispatch_send_to_client(*this, string_to_protocol(protocol_str), std::move(socket));
        return 0;
    }
}
