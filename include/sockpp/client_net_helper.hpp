#pragma once

namespace pokemon::tcp {

    template<typename T>
    class NETWORK_SOCKPP_POKEMON_API ClientNetHelper {
        public:
             ClientNetHelper() = default;


             [[nodiscard]] int send_request(T& t, std::shared_ptr<tcp::IConnection> socket) {
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
                std::cout << "send request: " << protocol_str << std::endl;
                m_dispatcher.dispatch_send_to_client(t, string_to_protocol(protocol_str), std::move(socket));

                 return 0;
             }

            command_dispatcher<T>& get_dispatcher() {
                 return m_dispatcher;
             }

          //   void initCommands() {
          /*  m_dispatcher.registerCommand(PROTOCOL::GET_IPS, std::make_unique<ip_command>());
            m_dispatcher.registerCommand(PROTOCOL::GET_PICS, std::make_unique<pictures_command>());
            m_dispatcher.registerCommand(PROTOCOL::GET_PIC, std::make_unique<image_data_command>());
            m_dispatcher.registerCommand(PROTOCOL::GET_ALIVE, std::make_unique<alive_command>());
            m_dispatcher.registerCommand(PROTOCOL::GET_ID, std::make_unique<add_new_node_command>()); */
       // }
    private:

        command_dispatcher<T> m_dispatcher;


    };
}


