#include <memory>

namespace pokemon::tcp
{

    int ClientNetHelper::send_request(std::shared_ptr<tcp::IConnection> socket) {
        if (!socket || !socket->is_open()) {
            return -1;
        }

        char protocol_buf[PROTOCOL_SIZE];

        // getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_CONNECTION, std::format(MSG_NODE_ID, getPort(), SERVER),
        //                                 socket->address(), getPort()));

        if (const auto res = socket->read(protocol_buf, sizeof(protocol_buf)); res <= 0) {
            return -1;
        }

        std::string protocol_str(protocol_buf, PROTOCOL_SIZE);
      //  getTrace().print(std::clog, std::format(MSG_SERVER_RECEIVED_QUERY, std::format(MSG_NODE_ID, getPort(), SERVER),
     //                                         protocol_str));

        //  m_dispatcher.dispatch_send_to_client(*this, string_to_protocol(protocol_str), std::move(socket));

        return 0;
    }
}


