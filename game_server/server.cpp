/**
 * server.cpp
 *
 *  by Chris, Jigar
 *
 */

#include "server.hpp"

int main(int argc, char *argv[1]) {
    if (argc != 2) {
        std::cout << "Usage ./server <port>" << std::endl;
        return 1;
    }

    try {
        boost::asio::io_service io_service;
        int port = atoi(argv[1]);
        //GameLoopServer loop_server(io_service, port);
        RegistrationServer registration_server(io_service, port);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
