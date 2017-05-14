#include "client.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;
        UDPClient client(io_service, argv[1], argv[2]);

        vector<int> keystrokes;
        keystrokes.push_back(1);
        keystrokes.push_back(3);

        client.send_keystrokes(keystrokes, 20);
        client.get_gamestate();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}