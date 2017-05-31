#include "client.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc < 3) {
            std::cerr << "Usage: client <host> <port> <action - register, loop>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;
        TCPClient registration_client(io_service, argv[1], argv[2]);
        UDPClient loop_client(io_service, argv[1], argv[2], 0);
        if (argc == 4) {
            if (!strcmp(argv[3], "register")) {
                std::cout << "register called!" << std::endl;
                int player_id = registration_client.register_player();
            }

            if (!strcmp(argv[3], "loop")) {
                std::cout << "loop called!" << std::endl;
                while (true) {
                    vector<int> keystrokes;
                    keystrokes.push_back(KC_I);
                    loop_client.send_keystrokes(keystrokes);
                    loop_client.get_gamestate();
                }
            }
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
