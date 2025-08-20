#include "pch.h"

namespace pokemon {

    Listen::Listen(const in_port_t port) : port(port) {
        auto listenThread = connect();
        listenThread.detach();
    }

    std::thread Listen::connect() {
        return std::thread([this] { listening(); });
    }

    int Listen::listening() {
        std::string id_str("[node - " + std::to_string(port) + "] - Listen");

        trace.print(std::clog, id_str + " - listening server started on port" + std::to_string(port));

        // Create acceptor
        sockpp::tcp_acceptor acc(port);
        if (!acc) {
            trace.print(std::cerr, id_str + " - listening server - Error creating the acceptor");
            //report_error(acc.last_error_str());
            return 1;
        }

        trace.print(std::clog,
                    id_str + " - listening server - Awaiting connections on port " + std::to_string(port) + "...");
      /*  while (true) {
            sockpp::inet_address address;
            if (auto res = acc.accept(&address); !res) {

            } else {
                trace.print(std::clog, id_str + " - listening Server started on port " + std::to_string(port));
                sockpp::tcp_socket socket = res.release();
                Server server(port);
                auto serverThread = server.run(socket);
                serverThread.join();
            }
        }
*/
        while (true) {
            // Accept a new client connection
            sockpp::tcp_socket sock = acc.accept();

            if (!sock) {
                trace.print(std::cerr, id_str + "- Error accepting incoming connection: " + acc.last_error_str());
                std::this_thread::sleep_for(threadSleep_s(1000, 2000));
            }
            else {
               std::cout<<"Listening on port: "<<port<<std::endl;
                trace.print(std::clog, id_str + " - listening Server started on port " + std::to_string(port));

                Server server(port);
                auto serverThread = server.run(sock);
                serverThread.join();


                // Create a thread and transfer the new stream to it.
              //  thread thr(run_echo, std::move(sock));
                //thr.detach();
            }
        }





    }
}
