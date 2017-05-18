/**
 * client_victor.cpp
 *
 * Will include the base logic for the code in terms of smooth movement, etc.
 *
 * It'll be easier to test this smaller file than ogre.
 */

#include "client_victor.hpp"

int main(int argc, char* argv[]) {
    try {
        boost::asio::io_service io_service;
        UDPClient client(io_service, "localhost", "8888");
        
        
        vector<int> keystrokes;
        keystrokes.push_back(0);
        keystrokes.push_back(1);
        keystrokes.push_back(2);
        keystrokes.push_back(3);

        client.send_keystrokes(keystrokes, 20);
        PiGameState state = client.get_gamestate();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
